// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>("FireAudioComponent");
	FireAudioComponent->SetupAttachment(WeaponMesh, MuzzleSocketName);
	FireAudioComponent->bAutoDestroy = false;
	FireAudioComponent->SetAutoActivate(false);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count must be more than zero"))
	checkf(DefaultAmmo.SpareBullets > 0, TEXT("SpareBullets count must be more than zero"))

	CurrentAmmo = DefaultAmmo;

	check(FireAudioComponent);
}

bool ASTUBaseWeapon::StartFire()
{
	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return false;
	}

	return true;
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::MakeShot()
{
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	const bool bPlayerViewPoint = GetPlayerViewPoint(ViewLocation, ViewRotation);
	if (!bPlayerViewPoint) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (!Character) return false;

	if (Character->IsPlayerControlled())
	{
		const auto Controller = Character->GetController<APlayerController>();
		if (!Controller) return false;
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
		ViewLocation = ViewLocation + ViewRotation.Vector() * 350; // Добавляем примерное расстояние от камеры до персонажа
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}

	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is already empty"));
		return;
	}

	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.SpareBullets == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}


void ASTUBaseWeapon::ChangeClip()
{
	if (CurrentAmmo.Infinite)
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}
	else
	{
		FillAmmo(CurrentAmmo.Bullets, DefaultAmmo.Bullets, CurrentAmmo.SpareBullets);
	}

	// UE_LOG(LogBaseWeapon, Display, TEXT("---- CHANGE CLIP ----"));
}

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.SpareBullets > 0;
}


bool ASTUBaseWeapon::TryToAddAmmo(const int32 BulletAmount)
{
	if (IsAmmoFull() || BulletAmount <= 0) return false;

	int32 RemainingBulletAmount = BulletAmount;
	FillAmmo(CurrentAmmo.Bullets, DefaultAmmo.Bullets, RemainingBulletAmount);
	FillAmmo(CurrentAmmo.SpareBullets, DefaultAmmo.SpareBullets, RemainingBulletAmount);

	return true;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.SpareBullets >= DefaultAmmo.SpareBullets && CurrentAmmo.Bullets >= DefaultAmmo.Bullets;
}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.SpareBullets);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() const
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,
	                                                    WeaponMesh,
	                                                    MuzzleSocketName,
	                                                    FVector::ZeroVector,
	                                                    FRotator::ZeroRotator,
	                                                    EAttachLocation::SnapToTarget,
	                                                    true);
}

void ASTUBaseWeapon::FillAmmo(int32& ValueToBeFilled, const int32& MaxValue, int32& ConsumableBulletAmount) const
{
	const int32 NeedToRefillAmount = MaxValue - ValueToBeFilled;
	if (NeedToRefillAmount <= 0 || ConsumableBulletAmount <= 0)
	{
		return;
	}

	const int32 Refilling = FMath::Min(ConsumableBulletAmount, NeedToRefillAmount);

	ConsumableBulletAmount -= Refilling;
	ValueToBeFilled += Refilling;
}

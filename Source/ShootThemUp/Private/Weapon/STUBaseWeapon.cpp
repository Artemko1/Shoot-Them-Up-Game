// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count must be more than zero"))
	checkf(DefaultAmmo.SpareBullets > 0, TEXT("SpareBullets count must be more than zero"))

	CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
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
	const auto Controller = GetPlayerController();
	if (!Controller) return false;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
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
		if (CurrentAmmo.SpareBullets == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No more spare bullets"));
			return;
		}

		const auto NeededAmmo = DefaultAmmo.Bullets - CurrentAmmo.Bullets;
		if (NeededAmmo <= 0)
		{
			constexpr auto LogMessage =
				"Reload is not needed because weapon has more bullets in current clip than it would get after reloadReload is not needed because weapon has more bullets in current clip than it would get after reloadReload is not needed because weapon has more bullets in current clip than it would get after reload";
			UE_LOG(LogBaseWeapon, Warning, TEXT("%s"), LogMessage);
			return;
		}

		const auto AmmoToBeRefilled = FMath::Min(NeededAmmo, CurrentAmmo.SpareBullets);
		checkf(AmmoToBeRefilled > 0, TEXT("Must refill positive amount of ammo!"))

		CurrentAmmo.SpareBullets -= AmmoToBeRefilled;
		CurrentAmmo.Bullets += AmmoToBeRefilled;
	}

	UE_LOG(LogBaseWeapon, Display, TEXT("---- CHANGE CLIP ----"));
}

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.SpareBullets > 0;
}


bool ASTUBaseWeapon::TryToAddAmmo(const int32 BulletAmount)
{
	if (IsAmmoFull() || BulletAmount <= 0) return false;

	// Локальная функция для заполнения одной шкалы патронов до максимума.
	auto FillAmmo = [](int32& ValueToBeFilled, const int32& MaxValue, int32& ConsumableBulletAmount)
	{
		const int32 NeedToRefillAmount = MaxValue - ValueToBeFilled;
		if (NeedToRefillAmount <= 0)
		{
			return;
		}

		const int32 Refilling = FMath::Min(ConsumableBulletAmount, NeedToRefillAmount);

		ConsumableBulletAmount -= Refilling;
		ValueToBeFilled += Refilling;
	};

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

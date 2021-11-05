// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "STUWeaponFXComponent.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All)

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
	DefaultAmmo = {25, 10, true};
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart;
	FVector TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd); // Кидает рейкаст из камеры до точки в центре экрана

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);

		WeaponFXComponent->PlayImpactFX(HitResult);
	}

	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	const bool bPlayerViewPoint = GetPlayerViewPoint(ViewLocation, ViewRotation);
	if (!bPlayerViewPoint) return false;

	TraceStart = ViewLocation + ViewRotation.Vector() * 350; // Добавляем примерное расстояние от камеры до персонажа

	const float HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	AActor* Target = HitResult.GetActor();
	if (!Target) return;

	Target->TakeDamage(DamageAmount, FPointDamageEvent(), GetController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(const bool IsVisible) const
{
	if (!MuzzleFXComponent)
	{
		return;
	}

	MuzzleFXComponent->SetPaused(!IsVisible);
	MuzzleFXComponent->SetVisibility(IsVisible);
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
	else
	{
		UE_LOG(LogRifleWeapon, Warning, TEXT("TraceFX niagara system is null"));
	}
}

AController* ASTURifleWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
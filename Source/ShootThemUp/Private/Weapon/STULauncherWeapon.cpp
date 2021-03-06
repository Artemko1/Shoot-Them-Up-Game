// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STULauncherWeapon.h"

#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/STUProjectile.h"

ASTULauncherWeapon::ASTULauncherWeapon()
{
	DefaultAmmo = {1, 5, false};
}

bool ASTULauncherWeapon::StartFire()
{
	if (Super::StartFire())
	{
		MakeShot();
		return true;
	}

	return false;
}

void ASTULauncherWeapon::MakeShot()
{
	if (!GetWorld()) return;

	FVector TraceStart;
	FVector TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
	// ReSharper disable once CppExpressionWithoutSideEffects
	SpawnMuzzleFX();

	if (FireAudioComponent)
	{
		FireAudioComponent->Play(0);
	}
}

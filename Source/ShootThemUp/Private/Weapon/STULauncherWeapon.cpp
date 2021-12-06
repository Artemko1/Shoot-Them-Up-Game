// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STULauncherWeapon.h"

#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon/STUProjectile.h"

ASTULauncherWeapon::ASTULauncherWeapon()
{
	DefaultAmmo = {1, 5, false};
}

void ASTULauncherWeapon::StartFire()
{
	Super::StartFire();

	if (IsAmmoEmpty())
	{
		return;
	}
	MakeShot();
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
	else
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}
}

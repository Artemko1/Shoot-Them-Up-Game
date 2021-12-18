// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USoundCue;
class UNiagaraComponent;
class UNiagaraSystem;
class USkeletalMeshComponent;

UCLASS(Abstract)
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTUBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	/**
	 * @brief 
	 * @return false if failed to start fire, true otherwise
	 */
	virtual bool StartFire();
	virtual void StopFire();

	void ChangeClip();
	bool CanReload() const;

	FWeaponUIData GetUIData() const { return UIData; }
	FAmmoData GetCurrentAmmoData() const { return CurrentAmmo; }
	bool TryToAddAmmo(int32 BulletAmount);
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo = {20, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

	virtual void BeginPlay() override;

	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;

	/**
	 * @brief Кидает рейкаст и возвращает Hit первого объекта на пути
	 */
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

	void DecreaseAmmo();
	bool IsClipEmpty() const;

	void LogAmmo();

	UNiagaraComponent* SpawnMuzzleFX() const;

private:
	FAmmoData CurrentAmmo;

	/// Заполняет одну шкалу патронов до максимума.
	void FillAmmo(int32& ValueToBeFilled, const int32& MaxValue, int32& ConsumableBulletAmount) const;
};

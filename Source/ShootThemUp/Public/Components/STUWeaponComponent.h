// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class UAnimMontage;
class ASTUBaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
	bool TryAddAmmo(const TSubclassOf<ASTUBaseWeapon> WeaponType, int32 BulletAmount);
	bool NeedAmmo(const TSubclassOf<ASTUBaseWeapon> WeaponType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (BlueprintBaseOnly))
	TArray<FWeaponData> WeaponData;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	/**
	 * @brief Список ссылок на все блупринты оружий, которые есть (созданы) в арсенале персонажа
	 */
	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	int32 CurrentWeaponIndex = 0;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CanFire() const;
	bool CanEquip() const;

	void EquipWeapon(int32 WeaponIndex);

private:
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	static void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void SpawnWeapons();

	void PlayAnimMontage(UAnimMontage* Animation) const;

	void InitAnimations();

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanReload() const;

	void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
	
	ASTUBaseWeapon* GetWeaponOfType(const TSubclassOf<ASTUBaseWeapon> WeaponType);
};


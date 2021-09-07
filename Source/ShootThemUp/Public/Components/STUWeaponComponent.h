// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (BlueprintBaseOnly))
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (BlueprintBaseOnly))
	UAnimMontage* ReloadAnimMontage;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (BlueprintBaseOnly))
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	/**
	 * @brief Список ссылок на все блупринты оружий, которые есть (созданы) в арсенале персонажа
	 */
	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0;
	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	static void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void SpawnWeapons();
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation) const;

	void InitAnimations();

	template <typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation) const
	{
		if (!Animation) return nullptr;

		const auto NotifyEvents = Animation->Notifies;
		for (const auto NotifyEvent : NotifyEvents)
		{
			const auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}

		return nullptr;
	}

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;
};

// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintGetter)
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0, ClampMax = 1000))
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	bool EnableAutoHeal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health",
		meta = (ClampMin = 0, ClampMax = 6, EditCondition = "EnableAutoHeal"))
	float HealUpdateTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health",
		meta = (ClampMin = 0, ClampMax = 120, EditCondition = "EnableAutoHeal"))
	float HealDelay = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health",
		meta = (ClampMin = 0, ClampMax = 100, EditCondition = "EnableAutoHeal"))
	float HealModifier = 1.0f;

	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

	FTimerHandle AutoHealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

	void AutoHealTick();
	void SetHealth(float NewHealth);
};

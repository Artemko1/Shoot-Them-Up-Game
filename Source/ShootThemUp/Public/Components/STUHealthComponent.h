// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUHealthComponent();

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintGetter, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintGetter, Category = "Health")
	bool IsFullHealth() const { return FMath::IsNearlyEqual(Health, MaxHealth); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool TryAddHealth(float Amount);

	float GetHealth() const { return Health; }

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

	FTimerHandle AutoHealTimerHandle;

	UFUNCTION()
	void ApplyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

	void AutoHealTick();
	void SetHealth(float NewHealth);
	void PlayCameraShake() const;

	void Killed(AController* KillerController) const;

	void ReportDamageEvent(float Damage, AController* InstigatedBy) const;
};

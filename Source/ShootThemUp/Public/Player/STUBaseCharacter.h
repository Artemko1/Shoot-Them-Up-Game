// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USoundCue;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual FVector GetTargetLocation(AActor* RequestedBy) const override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const { return false; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	void SetPlayerColor(const FLinearColor& Color) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnDeath();
	virtual void OnHealthChanged(float Health, float HealthDelta) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USceneComponent* TargetLocationPoint;

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};

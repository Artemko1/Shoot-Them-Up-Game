// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "GameFramework/Character.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

public:
	explicit ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsRunning() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	virtual void OnDeath() override;

private:
	bool bWantsToRun = false;
	bool bIsMovingForward = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void OnStartRunning();
	void OnStopRunning();
};

// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;
	
	virtual void BeginPlay() override;
};

// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UWWidgetWrappers/STUUWButton.h"
#include "STUUWGoToMenuButton.generated.h"


class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUUWGoToMenuButton : public USTUUWButton
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnGoToMenu();
};

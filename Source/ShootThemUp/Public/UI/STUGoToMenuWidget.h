// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUUWButton.h"
#include "STUGoToMenuWidget.generated.h"


class UButton;
UCLASS()
class SHOOTTHEMUP_API USTUGoToMenuWidget : public USTUUWButton // todo rename to USTUUWGoToMenuButton
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnGoToMenu();
};

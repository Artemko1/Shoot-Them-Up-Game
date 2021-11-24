// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPauseWidget.generated.h"

class USTUUWButton;

UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	USTUUWButton* ContinueUWButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();
};

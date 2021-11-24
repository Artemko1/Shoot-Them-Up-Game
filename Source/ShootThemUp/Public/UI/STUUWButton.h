// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUUWButton.generated.h"

class UImage;
class UButton;
class UTextBlock;
/**
 * Наследуемся из BP отсюда, чтобы сделать кнопку определённого стиля
 */
UCLASS()
class SHOOTTHEMUP_API USTUUWButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UButton* GetButton() const { return MainButton; }
	void SetText(FText InText) const;

protected:
	// Not all blueprint subclasses of this may want to have a text label
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* MainLabel;

	UPROPERTY(EditInstanceOnly, Category = "UI")
	FText MainLabelText;

	UPROPERTY(meta=(BindWidgetOptional))
	UImage* MainIcon;

	UPROPERTY(meta=(BindWidget))
	UButton* MainButton;

	virtual void SynchronizeProperties() override;
};

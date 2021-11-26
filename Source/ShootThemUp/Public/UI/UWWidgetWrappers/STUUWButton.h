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
	UTextBlock* GetText() const { return MainText; }
	UImage* GetImage() const { return MainImage; }

protected:
	UPROPERTY(meta=(BindWidget))
	UButton* MainButton;

	// Not all blueprint subclasses of this may want to have a text label
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* MainText;

	UPROPERTY(meta=(BindWidgetOptional))
	UImage* MainImage;

	virtual void SynchronizeProperties() override;

private:
	UPROPERTY(EditInstanceOnly, Category = "UI")
	FText TextBlockText;

	void SetText(FText InText) const;
};

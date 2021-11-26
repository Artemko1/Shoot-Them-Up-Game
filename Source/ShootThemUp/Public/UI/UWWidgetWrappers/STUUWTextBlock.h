// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUUWTextBlock.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUUWTextBlock : public UUserWidget
{
	GENERATED_BODY()

public:
	UTextBlock* GetText() const { return MainText; }

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MainText;

	virtual void SynchronizeProperties() override;

private:
	UPROPERTY(EditInstanceOnly, Category = "UI")
	FText TextBlockText;

	void SetText(FText InText) const;
};

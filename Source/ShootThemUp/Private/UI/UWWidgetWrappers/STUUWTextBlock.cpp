// Shoot Them Up Game. All Rights Reserved.


#include "UWWidgetWrappers/STUUWTextBlock.h"

#include "Components/TextBlock.h"

void USTUUWTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MainText && !TextBlockText.IsEmptyOrWhitespace())
	{
		SetText(TextBlockText);
	}
}

void USTUUWTextBlock::SetText(const FText InText) const
{
	if (MainText)
	{
		MainText->SetText(InText);
	}
}

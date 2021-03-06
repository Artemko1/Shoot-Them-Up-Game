// Shoot Them Up Game. All Rights Reserved.


#include "UWWidgetWrappers/STUUWButton.h"

#include "Components/TextBlock.h"

void USTUUWButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MainText && !TextBlockText.IsEmptyOrWhitespace())
	{
		SetText(TextBlockText);
	}
}

void USTUUWButton::SetText(const FText InText) const
{
	if (MainText)
	{
		MainText->SetText(InText);
	}
}

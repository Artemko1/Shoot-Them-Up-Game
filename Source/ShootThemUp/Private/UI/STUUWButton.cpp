// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUUWButton.h"

#include "Components/TextBlock.h"

void USTUUWButton::SetText(const FText InText) const
{
	if (MainLabel)
	{
		MainLabel->SetText(InText);
	}
}

void USTUUWButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MainLabel && !MainLabelText.IsEmptyOrWhitespace())
	{
		SetText(MainLabelText);
	}
}

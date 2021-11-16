// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"


void USTUPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
	}
}

void USTUPauseWidget::OnClearPause()
{
	if (!GetOwningPlayer()) return;

	GetOwningPlayer()->SetPause(false);
}

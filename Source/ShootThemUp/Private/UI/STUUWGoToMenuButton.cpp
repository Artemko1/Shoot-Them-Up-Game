// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUUWGoToMenuButton.h"

#include "StUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget, All, All);

void USTUUWGoToMenuButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MainButton)
	{
		MainButton->OnClicked.AddDynamic(this, &USTUUWGoToMenuButton::OnGoToMenu);
	}
}

void USTUUWGoToMenuButton::OnGoToMenu()
{
	if (!GetWorld()) return;

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if (!STUGameInstance) return;

	if (STUGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}

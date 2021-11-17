// Shoot Them Up Game. All Rights Reserved.


#include "Menu/UI/STUMenuWidget.h"

#include "StUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!StartGameButton) return;

	StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USTUMenuWidget::OnStartGame()
{
	if (!GetWorld()) return;

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if (!STUGameInstance) return;

	if (STUGameInstance->GetStartupLevelName().IsNone())
	{
		UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevelName());
}

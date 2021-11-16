// Shoot Them Up Game. All Rights Reserved.


#include "Menu/UI/STUMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!StartGameButton) return;

	StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
}

void USTUMenuWidget::OnStartGame()
{
	UE_LOG(LogTemp, Display, TEXT("OnStartGame"));
	const FName StartupLevelName = "TestLevel";
	UGameplayStatics::OpenLevel(this, StartupLevelName);
}

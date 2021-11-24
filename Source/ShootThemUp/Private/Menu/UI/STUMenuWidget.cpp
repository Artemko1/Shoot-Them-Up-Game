// Shoot Them Up Game. All Rights Reserved.


#include "Menu/UI/STUMenuWidget.h"

#include "STUGameInstance.h"
#include "STUUWButton.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/UI/STULevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameUWButton && StartGameUWButton->GetButton())
	{
		StartGameUWButton->GetButton()->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}

	InitLevelItems();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USTUMenuWidget::OnStartGame()
{
	const auto STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;

	// if (STUGameInstance->GetStartupLevelName().IsNone())
	// {
	// 	UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is NONE"));
	// 	return;
	// }

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;

	checkf(STUGameInstance->GetLevelsData()->Num() > 0, TEXT("Levels data must contain at least one level"))

	if (!LevelItemsBox) return;

	LevelItemsBox->ClearChildren();

	for (const FLevelData LevelData : *STUGameInstance->GetLevelsData())
	{
		USTULevelItemWidget* const LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (STUGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected((*STUGameInstance->GetLevelsData())[0]);
	}
	else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel());
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;

	STUGameInstance->SetStartupLevel(Data);

	for (USTULevelItemWidget* const LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData()->LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}

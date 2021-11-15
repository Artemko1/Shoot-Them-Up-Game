﻿// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUGameOverWidget.h"

#include "STUGameModeBase.h"
#include "STUPlayerState.h"
#include "STUPlayerStatRowWidget.h"
#include "STUUtils.h"
#include "Components/VerticalBox.h"

bool USTUGameOverWidget::Initialize()
{
	if (GetWorld())
	{
		const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
		}
	}

	return Super::Initialize();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USTUGameOverWidget::OnMatchStateChanged(const ESTUMatchState State)
{
	if (State == ESTUMatchState::GameOver)
	{
		UpdatePlayersStat();
	}
}

void USTUGameOverWidget::UpdatePlayersStat() const
{
	if (!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren();
	
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
		if (!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;

		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(FText::AsNumber(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(FText::AsNumber(PlayerState->GetDeathsNum()));
		PlayerStatRowWidget->SetTeam(FText::AsNumber(PlayerState->GetTeamID()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsLocalPlayerController());

		PlayerStatBox->AddChild(PlayerStatRowWidget);
	}
}

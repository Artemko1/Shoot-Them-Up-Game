// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetOwningPlayerState<ASTUPlayerState>();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetSTUGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetSTUGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetSTUGameMode();
	return GameMode ? GameMode->GetTotalRoundsNum() : 0;
}


ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetAuthGameMode<ASTUGameModeBase>() : nullptr;
}

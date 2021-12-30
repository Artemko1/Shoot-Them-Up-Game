// Shoot Them Up Game. All Rights Reserved.


#include "STUGameModeBase.h"
#include "EngineUtils.h"
#include "STUBaseCharacter.h"
#include "STUPlayerController.h"
#include "STUPlayerState.h"
#include "STURespawnComponent.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController);
	}
}

void ASTUGameModeBase::StartRound()
{
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::OnRoundEnd, GameData.RoundTime);
}

void ASTUGameModeBase::OnRoundEnd()
{
	if (CurrentRound + 1 <= GameData.RoundsNum)
	{
		++CurrentRound;
		ResetPlayers();
		StartRound();
	}
	else
	{
		GameOver();
	}
}

void ASTUGameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}

	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo() const
{
	if (!GetWorld()) return;

	int32 TeamID = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
		if (!PlayerState) continue;

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamId(TeamID));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		SetPlayerColor(Controller);

		TeamID = TeamID == 1 ? 2 : 1;
	}
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamId(const int32 TeamID) const
{
	if (TeamID > 0 && TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}

	UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID,
	       *GameData.DefaultTeamColor.ToString());

	return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;

	const auto Character = Controller->GetPawn<ASTUBaseCharacter>();
	if (!Character) return;

	const auto PlayerState = Character->GetPlayerState<ASTUPlayerState>();
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController) const
{
	const auto KillerPlayerState = KillerController ? KillerController->GetPlayerState<ASTUPlayerState>() : nullptr;
	const auto VictimPlayerState = VictimController ? VictimController->GetPlayerState<ASTUPlayerState>() : nullptr;

	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}

	StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if (PauseSet)
	{
		SetMatchState(ESTUMatchState::Pause);
	}

	return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
	const bool PauseCleared = Super::ClearPause();

	if (PauseCleared)
	{
		SetMatchState(ESTUMatchState::InProgress);
	}

	return PauseCleared;
}


void ASTUGameModeBase::LogPlayerInfo() const
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
		if (!PlayerState) continue;

		PlayerState->LogInfo();
	}
}

void ASTUGameModeBase::StartRespawn(const AController* Controller) const
{
	if (!GetWorld()) return;

	const auto TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(GameRoundTimerHandle);
	const auto IsRespawnAvailable = TimeRemaining > GameData.MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!IsRespawnAvailable) return;

	const auto RespawnComponent = Controller->FindComponentByClass<USTURespawnComponent>();
	if (!RespawnComponent) return;

	RespawnComponent->StartRespawn(GameData.RespawnTime);
}

void ASTUGameModeBase::GameOver()
{
	UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER ====="));
	LogPlayerInfo();

	for (const auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (!Pawn) continue;

		Pawn->TurnOff();
		Pawn->DisableInput(nullptr);
	}

	SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(const ESTUMatchState State)
{
	if (State == MatchState)
	{
		return;
	}

	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}

// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "STUGameModeBase.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTUGameModeBase();
	
	FMatchStateChangedSignature OnMatchStateChanged;
	
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void Killed(AController* KillerController, AController* VictimController) const;

	int32 GetCurrentRoundNum() const { return CurrentRound; }
	int32 GetTotalRoundsNum() const { return GameData.RoundsNum; }
	int32 GetRoundSecondsRemaining() const { return RoundCountDown; }
	void RespawnRequest(AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;

private:
	ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
	
	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundTimerHandle;

	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void CreateTeamsInfo() const;
	FLinearColor DetermineColorByTeamId(int32 TeamID) const;
	static void SetPlayerColor(AController* Controller);

	void LogPlayerInfo() const;

	void StartRespawn(const AController* Controller) const;

	void GameOver();

	void SetMatchState(const ESTUMatchState State);
};

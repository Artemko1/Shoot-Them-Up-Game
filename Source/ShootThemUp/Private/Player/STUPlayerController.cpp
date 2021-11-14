// Shoot Them Up Game. All Rights Reserved.


#include "Player/STUPlayerController.h"

#include "STUGameHUD.h"
#include "STUGameModeBase.h"
#include "STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
}

void ASTUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
		}
	}
}

void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
}

void ASTUPlayerController::OnPauseGame()
{
	SetPause(true);
}

void ASTUPlayerController::OnMatchStateChanged(const ESTUMatchState State)
{
	if (State == ESTUMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

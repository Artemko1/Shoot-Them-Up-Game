// Shoot Them Up Game. All Rights Reserved.


#include "STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(const int32 RespawnTime)
{
	if (!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.f,
	                                       true);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown == 0)
	{
		if (!GetWorld()) return;

		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
		if (!GameMode) return;

		GameMode->RespawnRequest(GetController());
	}
}

AController* USTURespawnComponent::GetController() const
{
	auto Controller = GetOwner<AController>();
	if (!Controller)
	{
		const auto Pawn = GetOwner<APawn>();
		Controller = Pawn ? Pawn->GetController() : nullptr;
	}

	return Controller;
}

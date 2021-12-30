// Shoot Them Up Game. All Rights Reserved.


#include "STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::StartRespawn(const int32 RespawnTime)
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerEnd, RespawnTime);
}

float USTURespawnComponent::GetRespawnCountDown() const
{
	if (!GetWorld()) return 0;

	return GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USTURespawnComponent::RespawnTimerEnd()
{
	if (!GetWorld()) return;

	const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
	if (!GameMode) return;

	GameMode->RespawnRequest(GetController());
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

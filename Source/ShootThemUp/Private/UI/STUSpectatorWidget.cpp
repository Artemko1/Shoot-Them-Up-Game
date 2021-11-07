// Shoot Them Up Game. All Rights Reserved.


#include "STUSpectatorWidget.h"

#include "STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController) return false;
	
	const auto RespawnComponent = PlayerController->FindComponentByClass<USTURespawnComponent>();
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}

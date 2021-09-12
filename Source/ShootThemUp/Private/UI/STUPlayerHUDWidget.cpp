// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player)
	{
		return .0f;
	}

	const auto HealthComponent = Cast<USTUHealthComponent>(Player->GetComponentByClass(USTUHealthComponent::StaticClass()));
	if (!HealthComponent)
	{
		return .0f; 
	}

	return HealthComponent->GetHealthPercent();
}

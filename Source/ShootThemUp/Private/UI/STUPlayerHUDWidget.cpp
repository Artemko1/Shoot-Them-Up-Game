// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return .0f;

	const auto HealthComponent = Cast<USTUHealthComponent>(
		Player->GetComponentByClass(USTUHealthComponent::StaticClass()));
	if (!HealthComponent)
	{
		return .0f;
	}

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return false;

	const auto WeaponComponent = Cast<USTUWeaponComponent>(
		Player->GetComponentByClass(USTUWeaponComponent::StaticClass()));
	if (!WeaponComponent) return false;

	return WeaponComponent->GetWeaponUIData(UIData);
}

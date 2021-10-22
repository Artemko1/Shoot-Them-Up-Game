// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

bool USTUPlayerHUDWidget::Initialize()
{
	const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
	}

	return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(const float Health, const float HealthDelta)
{
	if (HealthDelta < 0)
	{
		OnTakeDamage();
	}
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
	if (!HealthComponent) return .0f;

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}


bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

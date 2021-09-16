// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto Component = GetOwningPlayerPawnComponentByClass(USTUHealthComponent::StaticClass());
	const auto HealthComponent = Cast<USTUHealthComponent>(Component);
	if (!HealthComponent)
	{
		return .0f;
	}

	return HealthComponent->GetHealthPercent();
}


bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}


bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent) return false;
	
	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
	const auto Component = GetOwningPlayerPawnComponentByClass(USTUWeaponComponent::StaticClass());
	const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
	return WeaponComponent;
}


UActorComponent* USTUPlayerHUDWidget::GetOwningPlayerPawnComponentByClass(const TSubclassOf<UActorComponent> ComponentClass) const
{
	const auto Pawn = GetOwningPlayerPawn();
	if (!Pawn) return nullptr;

	const auto Component = Pawn->GetComponentByClass(ComponentClass);
	return Component;
}

// Shoot Them Up Game. All Rights Reserved.


#include "Pickups/STUHealthPickup.h"
#include "STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = PlayerPawn->FindComponentByClass<USTUHealthComponent>();
	if (!HealthComponent) return false;

	return HealthComponent->TryAddHealth(HealthAmount);
}

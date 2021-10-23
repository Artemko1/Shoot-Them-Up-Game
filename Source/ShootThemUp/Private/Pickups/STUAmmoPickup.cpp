// Shoot Them Up Game. All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	if (!PlayerPawn) return false;
	const auto HealthComponent = PlayerPawn->FindComponentByClass<USTUHealthComponent>();
	if (!HealthComponent || HealthComponent->IsDead()) return false;
	
	const auto WeaponComponent = PlayerPawn->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return false;
	
	return WeaponComponent->TryAddAmmo(WeaponType, BulletAmount);
}

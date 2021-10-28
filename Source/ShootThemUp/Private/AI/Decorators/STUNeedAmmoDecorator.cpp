// Shoot Them Up Game. All Rights Reserved.

#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
	NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return false;

	const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}

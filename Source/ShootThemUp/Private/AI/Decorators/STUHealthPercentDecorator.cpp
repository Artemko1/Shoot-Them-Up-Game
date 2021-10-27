// Shoot Them Up Game. All Rights Reserved.


#include "AI/Decorators/STUHealthPercentDecorator.h"

#include "AIController.h"
#include "STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
	NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return false;
	const auto HealthComponent = Pawn->FindComponentByClass<USTUHealthComponent>();
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}

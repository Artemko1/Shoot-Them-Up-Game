// Shoot Them Up Game. All Rights Reserved.

#include "AI/Services/STUChangeWeaponService.h"
#include "AIController.h"
#include "STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change weapon";
	Interval = 3.f;
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	const APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return;
	
	const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent || Probability <= 0) return;
	
	const auto Rand = FMath::FRand();
	if (Rand <= Probability)
	{
		WeaponComponent->NextWeapon();
	}

}

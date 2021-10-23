// Shoot Them Up Game. All Rights Reserved.


#include "AI/Services/STUFireService.h"

#include "AIController.h"
#include "STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Blackboard || !Controller) return;

	const auto HasAim = Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return;
	const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return;

	HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
}

// Shoot Them Up Game. All Rights Reserved.


#include "AI/Services/STUFindEnemyService.h"
#include "AIController.h"
#include "STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Blackboard || !Controller) return;

	const auto PerceptionComponent = Controller->FindComponentByClass<USTUAIPerceptionComponent>();
	if (!PerceptionComponent) return;

	Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
}

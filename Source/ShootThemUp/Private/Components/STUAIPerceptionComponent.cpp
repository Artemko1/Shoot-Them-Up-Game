// Shoot Them Up Game. All Rights Reserved.


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	if (PerceiveActors.Num() == 0) return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for (const auto PerceiveActor : PerceiveActors)
	{
		const auto HealthComponent = PerceiveActor->FindComponentByClass<USTUHealthComponent>();

		const auto PerceivePawn = Cast<APawn>(PerceiveActor);
		const auto AreEnemies = PerceivePawn && FSTUUtils::AreEnemies(Controller, PerceivePawn->GetController());
		
		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = FVector::Distance(PerceiveActor->GetActorLocation(), Pawn->GetActorLocation());
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}

	return BestPawn;
}

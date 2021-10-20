// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAIController.h"
#include "STUAIPerceptionComponent.h"
#include "AI/STUAICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAIController, All, All)

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUAICharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUAICharacter)
	{
		RunBehaviorTree(STUAICharacter->BehaviorTreeAsset);
	}
}

void ASTUAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const auto AimActor = STUAIPerceptionComponent->GetClosestEnemy();
	SetFocus(AimActor);
}

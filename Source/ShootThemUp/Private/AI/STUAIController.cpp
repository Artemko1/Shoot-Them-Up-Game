// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAIController.h"

#include "AI/STUAICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAIController, All, All)

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUAICharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUAICharacter)
	{
		RunBehaviorTree(STUAICharacter->BehaviorTreeAsset);
	}
}

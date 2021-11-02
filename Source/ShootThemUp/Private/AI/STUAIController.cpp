// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAIController.h"
#include "STUAIPerceptionComponent.h"
#include "AI/STUAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAIController, All, All)

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);

	bWantsPlayerState = true;
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
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUGameHUD.h"

#include "STUGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, Display, All);

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	// DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	const auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}

	if (GetWorld())
	{
		const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
		}

	}
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
	UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void ASTUGameHUD::DrawCrossHair()
{
	const FVector2D Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	constexpr float LineSize = 12.0f;
	constexpr float Offset = 6.0f;
	constexpr float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.X - Offset, Center.Y, Center.X - Offset - LineSize, Center.Y, LineColor, LineThickness);
	DrawLine(Center.X + Offset, Center.Y, Center.X + Offset + LineSize, Center.Y, LineColor, LineThickness);
	DrawLine(Center.X, Center.Y - Offset, Center.X, Center.Y - Offset - LineSize, LineColor, LineThickness);
	DrawLine(Center.X, Center.Y + Offset, Center.X, Center.Y + Offset + LineSize, LineColor, LineThickness);
}

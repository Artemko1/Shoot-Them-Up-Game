// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

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

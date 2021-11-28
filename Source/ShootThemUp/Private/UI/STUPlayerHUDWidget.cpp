// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "STUHealthComponent.h"
#include "STUPlayerState.h"
#include "STUWeaponComponent.h"
#include "Components/ProgressBar.h"


void USTUPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const auto Controller = GetOwningPlayer())
	{
		Controller->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn());
	}
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* Pawn)
{
	if (!Pawn) return;

	const auto HealthComponent = Pawn->FindComponentByClass<USTUHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
	}

	UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnHealthChanged(const float Health, const float HealthDelta)
{
	if (HealthDelta < 0)
	{
		OnTakeDamage();

		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	UpdateHealthBar();
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return false;
	const auto HealthComponent = Pawn->FindComponentByClass<USTUHealthComponent>();
	if (!HealthComponent) return .0f;

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return false;
	const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}


bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return false;
	const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) return false;
	const auto HealthComponent = Pawn->FindComponentByClass<USTUHealthComponent>();
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
	const auto PlayerState = GetOwningPlayerState<ASTUPlayerState>();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

FString USTUPlayerHUDWidget::FormatBullets(const int32 BulletsNum) const
{
	constexpr int32 MaxLen = 3;
	constexpr TCHAR PrefixSymbol = '0';

	FString BulletStr = FString::FromInt(BulletsNum);
	const int SymbolsNumToAdd = MaxLen - BulletStr.Len();

	if (SymbolsNumToAdd > 0)
	{
		BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
	}

	return BulletStr;
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
	}
}

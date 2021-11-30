// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUHealthComponent.h"

#include "STUGameModeBase.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::ApplyDamage);
	}
}

bool USTUHealthComponent::TryAddHealth(const float Amount)
{
	if (Amount <= 0 || IsDead() || IsFullHealth())
	{
		return false;
	}

	SetHealth(Health + Amount);
	UE_LOG(LogHealthComponent, Display, TEXT("Health was added"));
	return true;
}

void USTUHealthComponent::ApplyDamage(AActor* DamagedActor, const float Damage, const UDamageType* DamageType,
                                      AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) { return; }

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (EnableAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &USTUHealthComponent::AutoHealTick,
		                                       HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

void USTUHealthComponent::AutoHealTick()
{
	SetHealth(Health + HealModifier);
	if (IsFullHealth() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(const float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake() const
{
	if (IsDead())
	{
		return;
	}

	const auto Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}

	const auto Controller = Pawn->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager)
	{
		return;
	}

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController) const
{
	if (!GetWorld()) return;

	const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
	if (!GameMode) return;

	const auto Player = GetOwner<APawn>();
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

void USTUHealthComponent::ReportDamageEvent(const float Damage, AController* InstigatedBy) const
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;

	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(),
	                                   GetOwner()->GetActorLocation());
}

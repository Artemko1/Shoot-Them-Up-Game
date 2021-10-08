// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"

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
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
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

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, const float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) { return; }

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (EnableAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &USTUHealthComponent::AutoHealTick,
		                                       HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake();
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
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void USTUHealthComponent::PlayCameraShake()
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

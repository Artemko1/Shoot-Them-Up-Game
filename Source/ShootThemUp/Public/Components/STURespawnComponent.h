﻿// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTURespawnComponent();

	void StartRespawn(int32 RespawnTime);
	float GetRespawnCountDown() const;
	bool IsRespawnInProgress() const;

private:
	FTimerHandle RespawnTimerHandle;

	void RespawnTimerEnd();
	AController* GetController() const;
};

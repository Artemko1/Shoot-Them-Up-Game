// Shoot Them Up Game. All Rights Reserved.

#include "STUGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USTUGameInstance::ToggleVolume()
{
	if (!IsSoundMuted)
	{
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MuteSoundMix);
		IsSoundMuted = true;
	}
	else
	{
		UGameplayStatics::PopSoundMixModifier(GetWorld(), MuteSoundMix);
		IsSoundMuted = false;
	}
}

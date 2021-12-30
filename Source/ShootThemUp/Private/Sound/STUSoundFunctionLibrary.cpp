// Shoot Them Up Game. All Rights Reserved.

#include "Sound/STUSoundFunctionLibrary.h"

#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUSoundFunctionLibrary, All, All);

void USTUSoundFunctionLibrary::SetSoundClassVolume(USoundClass* SoundClass, const float Volume)
{
	if (!SoundClass) return;

	SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.f, 1.f);
	UE_LOG(LogSTUSoundFunctionLibrary, Display, TEXT("Sound class volume was changed: %s = %f"), *SoundClass->GetName(),
	       SoundClass->Properties.Volume);
}

void USTUSoundFunctionLibrary::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if (!SoundClass) return;

	const auto NextVolume = SoundClass->Properties.Volume > 0.f ? 0.f : 1.f;
	SetSoundClassVolume(SoundClass, NextVolume);
}
// Shoot Them Up Game. All Rights Reserved.

#include "STUGameInstance.h"

#include "Sound/STUSoundFunctionLibrary.h"

void USTUGameInstance::ToggleVolume() const
{
	USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}

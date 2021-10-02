#pragma once

#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

class FSTUUtils
{
public:
	template<typename T>
	static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};

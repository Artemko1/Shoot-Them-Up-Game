#pragma once
#include "STUPlayerState.h"

class FSTUUtils
{
public:
	bool static AreEnemies(AController* Controller1, AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

		const auto PlayerState1 = Controller1->GetPlayerState<ASTUPlayerState>();
		const auto PlayerState2 = Controller2->GetPlayerState<ASTUPlayerState>();
		if (!PlayerState1 || !PlayerState2) return false;

		return PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}
};

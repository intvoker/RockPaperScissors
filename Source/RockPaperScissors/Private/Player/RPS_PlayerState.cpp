// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_PlayerState.h"

void ARPS_PlayerState::Reset()
{
	Super::Reset();

	Wins = 0;
	Losses = 0;
	Ties = 0;
}

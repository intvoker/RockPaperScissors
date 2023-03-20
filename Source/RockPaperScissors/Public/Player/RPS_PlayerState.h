// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	int32 GetWins() const { return Wins; }
	void AddWin() { Wins++; }

	int32 GetLosses() const { return Losses; }
	void AddLoss() { Losses++; }

private:
	int32 Wins = 0;
	int32 Losses = 0;
};

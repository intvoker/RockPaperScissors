// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPS_Types.h"
#include "RPS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	int32 GetWins() const;
	void AddWin(int32 RoundIndex);

	int32 GetLosses() const;
	void AddLoss(int32 RoundIndex);

	int32 GetTies() const;
	void AddTie(int32 RoundIndex);

	void AddResult(int32 RoundIndex, ERPS_GameRoundResult Result);
	ERPS_GameRoundResult GetResult(int32 RoundIndex);

	virtual void Reset() override;

private:
	TMap<int32, ERPS_GameRoundResult> Results;

	int32 NumByResult(ERPS_GameRoundResult Result) const;
};

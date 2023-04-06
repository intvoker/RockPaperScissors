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
	void SetMatchResult(ERPS_GameMatchResult GameMatchResult);
	ERPS_GameMatchResult GetMatchResult() const;

	int32 GetWins() const;
	void AddWin(int32 RoundIndex, int32 PoseIndex);

	int32 GetLosses() const;
	void AddLoss(int32 RoundIndex, int32 PoseIndex);

	int32 GetTies() const;
	void AddTie(int32 RoundIndex, int32 PoseIndex);

	void AddRoundResult(int32 RoundIndex, int32 PoseIndex, ERPS_GameRoundResult GameRoundResult);
	TPair<int32, ERPS_GameRoundResult> GetRoundResult(int32 RoundIndex) const;

	virtual void Reset() override;

private:
	ERPS_GameMatchResult MatchResult;

	TMap<int32, TPair<int32, ERPS_GameRoundResult>> RoundResults;

	int32 NumByRoundResult(ERPS_GameRoundResult GameRoundResult) const;
};

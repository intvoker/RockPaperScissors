// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_PlayerState.h"

#include "Hands/RPS_Hand.h"
#include "RPS_Types.h"

int32 ARPS_PlayerState::GetWins() const
{
	return NumByResult(ERPS_GameRoundResult::Win);
}

void ARPS_PlayerState::AddWin(int32 RoundIndex, int32 PoseIndex)
{
	AddResult(RoundIndex, PoseIndex, ERPS_GameRoundResult::Win);
}

int32 ARPS_PlayerState::GetLosses() const
{
	return NumByResult(ERPS_GameRoundResult::Loss);
}

void ARPS_PlayerState::AddLoss(int32 RoundIndex, int32 PoseIndex)
{
	AddResult(RoundIndex, PoseIndex, ERPS_GameRoundResult::Loss);
}

int32 ARPS_PlayerState::GetTies() const
{
	return NumByResult(ERPS_GameRoundResult::Tie);
}

void ARPS_PlayerState::AddTie(int32 RoundIndex, int32 PoseIndex)
{
	AddResult(RoundIndex, PoseIndex, ERPS_GameRoundResult::Tie);
}

void ARPS_PlayerState::AddResult(int32 RoundIndex, int32 PoseIndex, ERPS_GameRoundResult GameRoundResult)
{
	Results.Add(RoundIndex, TPair<int32, ERPS_GameRoundResult>(PoseIndex, GameRoundResult));
}

TPair<int32, ERPS_GameRoundResult> ARPS_PlayerState::GetResult(int32 RoundIndex)
{
	const auto Result = Results.Find(RoundIndex);

	return Result
		? *Result
		: TPair<int32, ERPS_GameRoundResult>(ARPS_Hand::DefaultHandPoseIndex, ERPS_GameRoundResult::None);
}

void ARPS_PlayerState::Reset()
{
	Super::Reset();

	Results.Empty();
}

int32 ARPS_PlayerState::NumByResult(ERPS_GameRoundResult GameRoundResult) const
{
	const auto FoundResults = Results.FilterByPredicate(
		[GameRoundResult](const TPair<int32, TPair<int32, ERPS_GameRoundResult>>& Pair)
		{
			return Pair.Value.Value == GameRoundResult;
		});

	return FoundResults.Num();
}

// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_PlayerState.h"

#include "Hands/RPS_Hand.h"
#include "RPS_Types.h"

int32 ARPS_PlayerState::GetWins() const
{
	return NumByRoundResult(ERPS_GameRoundResult::Win);
}

void ARPS_PlayerState::AddWin(int32 RoundIndex, int32 PoseIndex)
{
	AddRoundResult(RoundIndex, PoseIndex, ERPS_GameRoundResult::Win);
}

int32 ARPS_PlayerState::GetLosses() const
{
	return NumByRoundResult(ERPS_GameRoundResult::Loss);
}

void ARPS_PlayerState::AddLoss(int32 RoundIndex, int32 PoseIndex)
{
	AddRoundResult(RoundIndex, PoseIndex, ERPS_GameRoundResult::Loss);
}

int32 ARPS_PlayerState::GetTies() const
{
	return NumByRoundResult(ERPS_GameRoundResult::Tie);
}

void ARPS_PlayerState::AddTie(int32 RoundIndex, int32 PoseIndex)
{
	AddRoundResult(RoundIndex, PoseIndex, ERPS_GameRoundResult::Tie);
}

void ARPS_PlayerState::AddRoundResult(int32 RoundIndex, int32 PoseIndex, ERPS_GameRoundResult GameRoundResult)
{
	RoundResults.Add(RoundIndex, TPair<int32, ERPS_GameRoundResult>(PoseIndex, GameRoundResult));
}

TPair<int32, ERPS_GameRoundResult> ARPS_PlayerState::GetRoundResult(int32 RoundIndex) const
{
	const auto RoundResult = RoundResults.Find(RoundIndex);

	return RoundResult
		? *RoundResult
		: TPair<int32, ERPS_GameRoundResult>(ARPS_Hand::DefaultHandPoseIndex, ERPS_GameRoundResult::None);
}

void ARPS_PlayerState::Reset()
{
	Super::Reset();

	MatchResult = ERPS_GameMatchResult::None;

	RoundResults.Empty();
}

int32 ARPS_PlayerState::NumByRoundResult(ERPS_GameRoundResult GameRoundResult) const
{
	const auto FoundRoundResults = RoundResults.FilterByPredicate(
		[GameRoundResult](const TPair<int32, TPair<int32, ERPS_GameRoundResult>>& Pair)
		{
			return Pair.Value.Value == GameRoundResult;
		});

	return FoundRoundResults.Num();
}

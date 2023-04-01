// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_PlayerState.h"

#include "RPS_Types.h"

int32 ARPS_PlayerState::GetWins() const
{
	return NumByResult(ERPS_GameRoundResult::Win);
}

void ARPS_PlayerState::AddWin(int32 RoundIndex)
{
	AddResult(RoundIndex, ERPS_GameRoundResult::Win);
}

int32 ARPS_PlayerState::GetLosses() const
{
	return NumByResult(ERPS_GameRoundResult::Loss);
}

void ARPS_PlayerState::AddLoss(int32 RoundIndex)
{
	AddResult(RoundIndex, ERPS_GameRoundResult::Loss);
}

int32 ARPS_PlayerState::GetTies() const
{
	return NumByResult(ERPS_GameRoundResult::Tie);
}

void ARPS_PlayerState::AddTie(int32 RoundIndex)
{
	AddResult(RoundIndex, ERPS_GameRoundResult::Tie);
}

void ARPS_PlayerState::AddResult(int32 RoundIndex, ERPS_GameRoundResult Result)
{
	Results.Add(RoundIndex, Result);
}

ERPS_GameRoundResult ARPS_PlayerState::GetResult(int32 RoundIndex)
{
	const auto Result = Results.Find(RoundIndex);

	return Result ? *Result : ERPS_GameRoundResult::None;
}

void ARPS_PlayerState::Reset()
{
	Super::Reset();

	Results.Empty();
}

int32 ARPS_PlayerState::NumByResult(ERPS_GameRoundResult Result) const
{
	const auto FoundResults = Results.FilterByPredicate([Result](const TPair<int32, ERPS_GameRoundResult>& Pair)
	{
		return Pair.Value == Result;
	});

	return FoundResults.Num();
}

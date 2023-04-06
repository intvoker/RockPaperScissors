// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameMatchEndedWidget.h"

#include "Player/RPS_PlayerState.h"
#include "RPS_GameModeBase.h"

FText URPS_GameMatchEndedWidget::GetMatchResultInfo()
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RPS_PlayerState = RPS_GameModeBase->GetPlayerState();
	if (!RPS_PlayerState)
		return FText::GetEmpty();

	const auto RPS_AIPlayerState = RPS_GameModeBase->GetAIPlayerState();
	if (!RPS_AIPlayerState)
		return FText::GetEmpty();

	const auto PlayerMatchResult = RPS_PlayerState->GetMatchResult();
	const auto PlayerGameMatchResult = UEnum::GetDisplayValueAsText(PlayerMatchResult).ToString();

	const auto PlayerMatchResultInfo = GetPlayerStateMatchResultInfo(RPS_PlayerState);
	const auto AIPlayerMatchResultInfo = GetPlayerStateMatchResultInfo(RPS_AIPlayerState);

	const auto MatchResultInfo = FString::Printf(TEXT("%s%s%s%s%s"), *PlayerGameMatchResult, LINE_TERMINATOR,
	                                             *PlayerMatchResultInfo, LINE_TERMINATOR, *AIPlayerMatchResultInfo);

	return FText::FromString(MatchResultInfo);
}

FString URPS_GameMatchEndedWidget::GetPlayerStateMatchResultInfo(const ARPS_PlayerState* RPS_PlayerState)
{
	const auto MatchResult = RPS_PlayerState->GetMatchResult();
	const auto GameMatchResult = UEnum::GetDisplayValueAsText(MatchResult).ToString();

	return FString::Printf(
		TEXT("%s Wins: %d. Losses: %d. Ties: %d. Match result: %s."), *RPS_PlayerState->GetPlayerName(),
		RPS_PlayerState->GetWins(), RPS_PlayerState->GetLosses(), RPS_PlayerState->GetTies(), *GameMatchResult);
}

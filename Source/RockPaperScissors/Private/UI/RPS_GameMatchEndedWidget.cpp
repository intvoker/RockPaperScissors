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

	const auto PlayerMatchResultInfo = FString::Printf(
		TEXT("Player Wins: %d. Losses: %d. Ties: %d"), RPS_PlayerState->GetWins(), RPS_PlayerState->GetLosses(),
		RPS_PlayerState->GetTies());

	const auto AIPlayerMatchResult = RPS_AIPlayerState->GetMatchResult();
	const auto AIPlayerGameMatchResult = UEnum::GetDisplayValueAsText(AIPlayerMatchResult).ToString();

	const auto AIPlayerMatchResultInfo = FString::Printf(
		TEXT("AI Player Wins: %d. Losses: %d. Ties: %d"), RPS_AIPlayerState->GetWins(), RPS_AIPlayerState->GetLosses(),
		RPS_AIPlayerState->GetTies());

	const auto MatchResultInfo = FString::Printf(TEXT("%s%s%s%s%s"), *PlayerGameMatchResult, LINE_TERMINATOR,
	                                             *PlayerMatchResultInfo, LINE_TERMINATOR, *AIPlayerMatchResultInfo);

	return FText::FromString(MatchResultInfo);
}

// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameRoundEndedWidget.h"

#include "Player/RPS_PlayerState.h"
#include "RPS_GameModeBase.h"

FText URPS_GameRoundEndedWidget::GetRoundResultInfo()
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

	const auto PlayerRoundResult = RPS_PlayerState->GetRoundResult(RPS_GameModeBase->GetCurrentRoundIndex());
	const auto PlayerGameRoundResult = UEnum::GetDisplayValueAsText(PlayerRoundResult.Value).ToString();

	const auto PlayerRoundResultInfo = FString::Printf(
		TEXT("Player Pose: %s. Round result: %s."), *RPS_GameModeBase->GetHandPoseName(PlayerRoundResult.Key),
		*PlayerGameRoundResult);

	const auto AIPPlayerRoundResult = RPS_AIPlayerState->GetRoundResult(RPS_GameModeBase->GetCurrentRoundIndex());
	const auto AIPlayerGameRoundResult = UEnum::GetDisplayValueAsText(AIPPlayerRoundResult.Value).ToString();

	const auto AIPlayerRoundResultInfo = FString::Printf(
		TEXT("AI Player Pose: %s. Round result: %s."), *RPS_GameModeBase->GetHandPoseName(AIPPlayerRoundResult.Key),
		*AIPlayerGameRoundResult);

	const auto RoundResultInfo = FString::Printf(TEXT("%s%s%s%s%s"), *PlayerGameRoundResult, LINE_TERMINATOR,
	                                             *PlayerRoundResultInfo, LINE_TERMINATOR, *AIPlayerRoundResultInfo);

	return FText::FromString(RoundResultInfo);
}

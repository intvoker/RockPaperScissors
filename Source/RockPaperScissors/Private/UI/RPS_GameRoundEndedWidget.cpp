// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameRoundEndedWidget.h"

#include "RPS_GameModeBase.h"
#include "Player/RPS_PlayerState.h"

FText URPS_GameRoundEndedWidget::GetResultInfo()
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

	const auto PlayerResult = RPS_PlayerState->GetResult(RPS_GameModeBase->GetCurrentRoundIndex());
	const auto PlayerGameRoundResult = UEnum::GetDisplayValueAsText(PlayerResult.Value).ToString();

	const auto PlayerResultInfo = FString::Printf(
		TEXT("Player pose: %s. Result: %s."), *RPS_GameModeBase->GetHandPoseName(PlayerResult.Key),
		*PlayerGameRoundResult);

	const auto AIPPlayerResult = RPS_AIPlayerState->GetResult(RPS_GameModeBase->GetCurrentRoundIndex());
	const auto AIPlayerGameRoundResult = UEnum::GetDisplayValueAsText(AIPPlayerResult.Value).ToString();

	const auto AIPlayerResultInfo = FString::Printf(
		TEXT("AI Player pose: %s. Result: %s."), *RPS_GameModeBase->GetHandPoseName(AIPPlayerResult.Key),
		*AIPlayerGameRoundResult);

	const auto ResultInfo = FString::Printf(TEXT("%s%s%s%s%s"), *PlayerGameRoundResult, LINE_TERMINATOR,
	                                        *PlayerResultInfo, LINE_TERMINATOR, *AIPlayerResultInfo);

	return FText::FromString(ResultInfo);
}

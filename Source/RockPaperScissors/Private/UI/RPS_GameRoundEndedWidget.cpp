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
	const auto PlayerGameRoundResultInfo = FString::Printf(TEXT("It's a %s!"), *PlayerGameRoundResult);

	const auto PlayerRoundResultInfo = GetPlayerStateRoundResultInfo(RPS_GameModeBase, RPS_PlayerState);
	const auto AIPlayerRoundResultInfo = GetPlayerStateRoundResultInfo(RPS_GameModeBase, RPS_AIPlayerState);

	const auto RoundResultInfo = FString::Printf(TEXT("%s%s%s%s%s"), *PlayerGameRoundResultInfo, LINE_TERMINATOR,
	                                             *PlayerRoundResultInfo, LINE_TERMINATOR, *AIPlayerRoundResultInfo);

	return FText::FromString(RoundResultInfo);
}

FString URPS_GameRoundEndedWidget::GetPlayerStateRoundResultInfo(const ARPS_GameModeBase* RPS_GameModeBase,
                                                                 const ARPS_PlayerState* RPS_PlayerState)
{
	const auto RoundResult = RPS_PlayerState->GetRoundResult(RPS_GameModeBase->GetCurrentRoundIndex());

	return FString::Printf(TEXT("%s - %s"), *RPS_PlayerState->GetPlayerName(),
	                       *RPS_GameModeBase->GetHandPoseName(RoundResult.Key));
}

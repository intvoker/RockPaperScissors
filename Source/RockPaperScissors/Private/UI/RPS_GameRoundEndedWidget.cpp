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

	const auto Result = RPS_PlayerState->GetResult(RPS_GameModeBase->GetCurrentRoundIndex());
	if (Result == ERPS_GameRoundResult::None)
		return FText::GetEmpty();

	return UEnum::GetDisplayValueAsText(Result);
}

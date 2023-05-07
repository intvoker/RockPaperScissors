// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameRoundEndedWidget.h"

#include "Components/TextBlock.h"
#include "Player/RPS_PlayerState.h"
#include "RPS_GameModeBase.h"

void URPS_GameRoundEndedWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnNativeVisibilityChanged.AddUObject(this, &ThisClass::HandleOnNativeVisibilityChanged);
}

void URPS_GameRoundEndedWidget::HandleOnNativeVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility != ESlateVisibility::Visible)
		return;

	RoundResultTextBlock->SetText(GetRoundResultInfo());
	RoundStatsTextBlock->SetText(GetRoundStatsInfo());
}

FText URPS_GameRoundEndedWidget::GetRoundResultInfo() const
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RPS_PlayerState = RPS_GameModeBase->GetPlayerState();
	if (!RPS_PlayerState)
		return FText::GetEmpty();

	const auto PlayerRoundResult = RPS_PlayerState->GetRoundResult(RPS_GameModeBase->GetCurrentRoundIndex());
	const auto PlayerGameRoundResult = UEnum::GetDisplayValueAsText(PlayerRoundResult.Value).ToString();
	const auto PlayerGameRoundResultInfo = FString::Printf(TEXT("It's a %s!"), *PlayerGameRoundResult);

	return FText::FromString(PlayerGameRoundResultInfo);
}

FText URPS_GameRoundEndedWidget::GetRoundStatsInfo() const
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

	const auto PlayerRoundStatsInfo = GetPlayerStateRoundStatsInfo(RPS_GameModeBase, RPS_PlayerState);
	const auto AIPlayerRoundStatsInfo = GetPlayerStateRoundStatsInfo(RPS_GameModeBase, RPS_AIPlayerState);

	const auto RoundStatsInfo = FString::Printf(TEXT("%s%s%s"), *PlayerRoundStatsInfo, LINE_TERMINATOR,
	                                            *AIPlayerRoundStatsInfo);

	return FText::FromString(RoundStatsInfo);
}

FString URPS_GameRoundEndedWidget::GetPlayerStateRoundStatsInfo(const ARPS_GameModeBase* RPS_GameModeBase,
                                                                 const ARPS_PlayerState* RPS_PlayerState)
{
	const auto RoundResult = RPS_PlayerState->GetRoundResult(RPS_GameModeBase->GetCurrentRoundIndex());

	return FString::Printf(TEXT("%s - %s"), *RPS_PlayerState->GetPlayerName(),
	                       *RPS_GameModeBase->GetHandPoseName(RoundResult.Key));
}

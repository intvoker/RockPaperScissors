// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameMatchEndedWidget.h"

#include "Components/TextBlock.h"
#include "Player/RPS_PlayerState.h"
#include "RPS_GameModeBase.h"

void URPS_GameMatchEndedWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnNativeVisibilityChanged.AddUObject(this, &ThisClass::HandleOnNativeVisibilityChanged);
}

void URPS_GameMatchEndedWidget::HandleOnNativeVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility != ESlateVisibility::Visible)
		return;

	MatchResultTextBlock->SetText(GetMatchResultInfo());
	MatchStatsTextBlock->SetText(GetMatchStatsInfo());
}

FText URPS_GameMatchEndedWidget::GetMatchResultInfo() const
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RPS_PlayerState = RPS_GameModeBase->GetPlayerState();
	if (!RPS_PlayerState)
		return FText::GetEmpty();

	const auto PlayerMatchResult = RPS_PlayerState->GetMatchResult();
	const auto PlayerGameMatchResult = UEnum::GetDisplayValueAsText(PlayerMatchResult).ToString();
	const auto PlayerGameMatchResultInfo = FString::Printf(TEXT("It's a %s!"), *PlayerGameMatchResult);

	return FText::FromString(PlayerGameMatchResultInfo);
}

FText URPS_GameMatchEndedWidget::GetMatchStatsInfo() const
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

	const auto PlayerMatchStatsInfo = GetPlayerStateMatchStatsInfo(RPS_PlayerState);
	const auto AIPlayerMatchStatsInfo = GetPlayerStateMatchStatsInfo(RPS_AIPlayerState);

	const auto MatchStatsInfo = FString::Printf(TEXT("%s%s%s"), *PlayerMatchStatsInfo, LINE_TERMINATOR,
	                                            *AIPlayerMatchStatsInfo);

	return FText::FromString(MatchStatsInfo);
}

FString URPS_GameMatchEndedWidget::GetPlayerStateMatchStatsInfo(const ARPS_PlayerState* RPS_PlayerState)
{
	return FString::Printf(TEXT("%s - %d Wins, %d Losses, %d Ties"), *RPS_PlayerState->GetPlayerName(),
	                       RPS_PlayerState->GetWins(), RPS_PlayerState->GetLosses(), RPS_PlayerState->GetTies());
}

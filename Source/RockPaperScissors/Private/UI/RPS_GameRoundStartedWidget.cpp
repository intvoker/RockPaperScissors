// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameRoundStartedWidget.h"

#include "RPS_GameModeBase.h"

FText URPS_GameRoundStartedWidget::GetRoundInfo()
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RoundNumber = FString::Printf(TEXT("Round %d / %d"), RPS_GameModeBase->GetCurrentRoundIndex(),
	                                         RPS_GameModeBase->GetNumberOfRounds());
	const auto RoundRemainingSeconds = FString::Printf(
		TEXT("Remaining %d seconds"), RPS_GameModeBase->GetCurrentRoundRemainingSeconds());

	const auto RoundInfo = FString::Printf(TEXT("%s%s%s"), *RoundNumber, LINE_TERMINATOR, *RoundRemainingSeconds);

	return FText::FromString(RoundInfo);
}

// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameRoundStartedWidget.h"

#include "RPS_GameModeBase.h"

FText URPS_GameRoundStartedWidget::GetRoundInfo()
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RoundInfo = FString::Printf(
		TEXT("Round: %d / %d. Remaining %d seconds."), RPS_GameModeBase->GetCurrentRoundIndex(),
		RPS_GameModeBase->GetNumberOfRounds(), RPS_GameModeBase->GetCurrentRoundRemainingSeconds());

	return FText::FromString(RoundInfo);
}

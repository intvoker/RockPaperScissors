// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_GameRoundStartedWidget.h"

#include "Components/TextBlock.h"
#include "RPS_GameModeBase.h"

void URPS_GameRoundStartedWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnNativeVisibilityChanged.AddUObject(this, &ThisClass::HandleOnNativeVisibilityChanged);

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->OnRoundRemainingSecondsChanged.AddDynamic(
			this, &ThisClass::HandleOnRoundRemainingSecondsChanged);
	}
}

void URPS_GameRoundStartedWidget::HandleOnNativeVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility != ESlateVisibility::Visible)
		return;

	RoundNumberTextBlock->SetText(GetRoundNumberInfo());
	RoundRemainingSecondsTextBlock->SetText(GetRoundRemainingSecondsInfo());
}

void URPS_GameRoundStartedWidget::HandleOnRoundRemainingSecondsChanged(int32 RoundRemainingSeconds)
{
	RoundRemainingSecondsTextBlock->SetText(GetRoundRemainingSecondsInfo());
}

FText URPS_GameRoundStartedWidget::GetRoundNumberInfo() const
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RoundNumber = FString::Printf(TEXT("Round %d / %d"), RPS_GameModeBase->GetCurrentRoundIndex(),
	                                         RPS_GameModeBase->GetNumberOfRounds());

	return FText::FromString(RoundNumber);
}

FText URPS_GameRoundStartedWidget::GetRoundRemainingSecondsInfo() const
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	const auto RoundRemainingSeconds = FString::Printf(TEXT("%d"), RPS_GameModeBase->GetCurrentRoundRemainingSeconds());

	return FText::FromString(RoundRemainingSeconds);
}

// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_StartRoundWidget.h"

#include "Components/TextBlock.h"
#include "RPS_GameModeBase.h"

void URPS_StartRoundWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetStartRoundInfo();
}

void URPS_StartRoundWidget::SetStartRoundInfo() const
{
	if (!StartRoundTextBlock)
		return;

	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return;

	const auto StartRoundInfo = FString::Printf(
		TEXT("Show %s to start a new round"), *RPS_GameModeBase->GetStartRoundHandPoseName());

	StartRoundTextBlock->SetText(FText::FromString(StartRoundInfo));
}

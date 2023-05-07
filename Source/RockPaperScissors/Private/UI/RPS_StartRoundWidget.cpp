// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_StartRoundWidget.h"

#include "Components/TextBlock.h"
#include "RPS_GameModeBase.h"

void URPS_StartRoundWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartRoundTextBlock)
	{
		StartRoundTextBlock->SetText(GetStartRoundInfo());
	}
}

FText URPS_StartRoundWidget::GetStartRoundInfo() const
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	//const auto StartRoundHandPoseName = RPS_GameModeBase->GetStartRoundHandPoseName();
	const auto StartRoundHandPoseName = TEXT("Thumb Up");

	const auto StartRoundInfo = FString::Printf(TEXT("Use %s to start a new round"), StartRoundHandPoseName);

	return FText::FromString(StartRoundInfo);
}

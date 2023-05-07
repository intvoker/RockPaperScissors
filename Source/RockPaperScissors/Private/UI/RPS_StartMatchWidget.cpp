// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_StartMatchWidget.h"

#include "Components/TextBlock.h"
#include "RPS_GameModeBase.h"

void URPS_StartMatchWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartMatchTextBlock)
	{
		StartMatchTextBlock->SetText(GetStartMatchInfo());
	}
}

FText URPS_StartMatchWidget::GetStartMatchInfo() const
{
	const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>();
	if (!RPS_GameModeBase)
		return FText::GetEmpty();

	//TArray<FInputActionKeyMapping> OutMappings;
	//GetDefault<UInputSettings>()->GetActionMappingByName("StartMatch", OutMappings);
	//const auto StartMatchHandPoseName = OutMappings[0].Key.ToString();
	const auto StartMatchHandPoseName = TEXT("left hand Pinky Pinch");

	const auto StartMatchInfo = FString::Printf(TEXT("Use %s to start a new match"), StartMatchHandPoseName);

	return FText::FromString(StartMatchInfo);
}

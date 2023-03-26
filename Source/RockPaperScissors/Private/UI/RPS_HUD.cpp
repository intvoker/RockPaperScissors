// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_HUD.h"

#include "Blueprint/UserWidget.h"
#include "RPS_GameModeBase.h"
#include "UI/RPS_WidgetActor.h"

void ARPS_HUD::BeginPlay()
{
	Super::BeginPlay();

	GameMatchWidgets.Add(ERPS_GameMatchState::Started,
	                     CreateWidget<UUserWidget>(GetWorld(), GameMatchStartedWidgetClass));
	GameMatchWidgets.Add(ERPS_GameMatchState::Ended,
	                     CreateWidget<UUserWidget>(GetWorld(), GameMatchEndedWidgetClass));

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->OnGameMatchStateChanged.AddDynamic(this, &ThisClass::HandleOnGameMatchStateChanged);
	}
}

void ARPS_HUD::HandleOnGameMatchStateChanged(ERPS_GameMatchState GameMatchState)
{
	UUserWidget* CurrentWidget = nullptr;

	if (GameMatchWidgets.Contains(GameMatchState))
	{
		CurrentWidget = GameMatchWidgets[GameMatchState];
	}

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->GetWidgetActor()->SetWidget(CurrentWidget);
	}
}

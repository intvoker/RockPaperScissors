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

	GameRoundWidgets.Add(ERPS_GameRoundState::Started,
	                     CreateWidget<UUserWidget>(GetWorld(), GameRoundStartedWidgetClass));
	GameRoundWidgets.Add(ERPS_GameRoundState::Ended,
	                     CreateWidget<UUserWidget>(GetWorld(), GameRoundEndedWidgetClass));

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->OnGameMatchStateChanged.AddDynamic(this, &ThisClass::HandleOnGameMatchStateChanged);
		RPS_GameModeBase->OnGameRoundStateChanged.AddDynamic(this, &ThisClass::HandleOnGameRoundStateChanged);
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

void ARPS_HUD::HandleOnGameRoundStateChanged(ERPS_GameRoundState GameRoundState)
{
	UUserWidget* CurrentWidget = nullptr;

	if (GameRoundWidgets.Contains(GameRoundState))
	{
		CurrentWidget = GameRoundWidgets[GameRoundState];
	}

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->GetWidgetActor()->SetWidget(CurrentWidget);
	}
}

// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_HUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/StereoLayerFunctionLibrary.h"
#include "OculusFunctionLibrary.h"
#include "RPS_GameModeBase.h"
#include "UI/RPS_WidgetActor.h"

void ARPS_HUD::BeginPlay()
{
	Super::BeginPlay();

	GameMatchWidgets.Add(ERPS_GameMatchState::Started, CreateGameWidget(GameMatchStartedWidgetClass));
	GameMatchWidgets.Add(ERPS_GameMatchState::Ended, CreateGameWidget(GameMatchEndedWidgetClass));

	GameRoundWidgets.Add(ERPS_GameRoundState::Started, CreateGameWidget(GameRoundStartedWidgetClass));
	GameRoundWidgets.Add(ERPS_GameRoundState::Ended, CreateGameWidget(GameRoundEndedWidgetClass));

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->OnGameMatchStateChanged.AddDynamic(this, &ThisClass::HandleOnGameMatchStateChanged);
		RPS_GameModeBase->OnGameRoundStateChanged.AddDynamic(this, &ThisClass::HandleOnGameRoundStateChanged);
	}

	ShowSplashScreen();
}

void ARPS_HUD::HandleOnGameMatchStateChanged(ERPS_GameMatchState GameMatchState)
{
	if (GameMatchWidgets.Contains(GameMatchState))
	{
		SetCurrentGameWidget(GameMatchWidgets[GameMatchState]);
	}
}

void ARPS_HUD::HandleOnGameRoundStateChanged(ERPS_GameRoundState GameRoundState)
{
	if (GameRoundWidgets.Contains(GameRoundState))
	{
		SetCurrentGameWidget(GameRoundWidgets[GameRoundState]);
	}
}

UUserWidget* ARPS_HUD::CreateGameWidget(TSubclassOf<UUserWidget> WidgetClass) const
{
	const auto GameWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

	GameWidget->SetVisibility(ESlateVisibility::Hidden);

	return GameWidget;
}

void ARPS_HUD::SetCurrentGameWidget(UUserWidget* InWidget)
{
	if (CurrentGameWidget)
	{
		CurrentGameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentGameWidget = InWidget;

	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->GetWidgetActor()->SetWidget(CurrentGameWidget);
	}

	if (CurrentGameWidget)
	{
		CurrentGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARPS_HUD::ShowSplashScreen()
{
	if (!SplashScreen)
		return;

	UOculusFunctionLibrary::AddLoadingSplashScreen(SplashScreen, SplashScreenTranslationInMeters, FRotator::ZeroRotator,
	                                               SplashScreenSizeInMeters);

	UStereoLayerFunctionLibrary::ShowSplashScreen();

	GetWorld()->GetTimerManager().SetTimer(SplashScreenTimerHandle, this, &ThisClass::HideSplashScreen,
	                                       SplashScreenDelay);
}

void ARPS_HUD::HideSplashScreen()
{
	UStereoLayerFunctionLibrary::HideSplashScreen();
}

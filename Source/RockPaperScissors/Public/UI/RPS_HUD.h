// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPS_Types.h"
#include "RPS_HUD.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameMatchStartedWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameMatchEndedWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameRoundStartedWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameRoundEndedWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* SplashScreen;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector SplashScreenTranslationInMeters = FVector(4.0f, 0.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector2D SplashScreenSizeInMeters = FVector2D(3.0f, 3.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float SplashScreenDelay = 5.0f;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UUserWidget* CurrentGameWidget = nullptr;

	UPROPERTY()
	TMap<ERPS_GameMatchState, UUserWidget*> GameMatchWidgets;

	UPROPERTY()
	TMap<ERPS_GameRoundState, UUserWidget*> GameRoundWidgets;

	FTimerHandle SplashScreenTimerHandle;

	UFUNCTION()
	void HandleOnGameMatchStateChanged(ERPS_GameMatchState GameMatchState);

	UFUNCTION()
	void HandleOnGameRoundStateChanged(ERPS_GameRoundState GameRoundState);

	UUserWidget* CreateGameWidget(TSubclassOf<UUserWidget> WidgetClass) const;

	void SetCurrentGameWidget(UUserWidget* InWidget);

	void ShowSplashScreen();
	void HideSplashScreen();
};

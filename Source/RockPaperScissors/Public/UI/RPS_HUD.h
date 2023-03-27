// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<ERPS_GameMatchState, UUserWidget*> GameMatchWidgets;

	UPROPERTY()
	TMap<ERPS_GameRoundState, UUserWidget*> GameRoundWidgets;

	UFUNCTION()
	void HandleOnGameMatchStateChanged(ERPS_GameMatchState GameMatchState);

	UFUNCTION()
	void HandleOnGameRoundStateChanged(ERPS_GameRoundState GameRoundState);
};

// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPS_GameRoundEndedWidget.generated.h"

class ARPS_GameModeBase;
class ARPS_PlayerState;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_GameRoundEndedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FText GetRoundResultInfo();

private:
	static FString GetPlayerStateRoundResultInfo(const ARPS_GameModeBase* RPS_GameModeBase,
	                                             const ARPS_PlayerState* RPS_PlayerState);
};

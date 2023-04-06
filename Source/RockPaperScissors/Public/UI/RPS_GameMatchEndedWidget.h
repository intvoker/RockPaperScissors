// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPS_GameMatchEndedWidget.generated.h"

class ARPS_PlayerState;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_GameMatchEndedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FText GetMatchResultInfo();

private:
	static FString GetPlayerStateMatchResultInfo(const ARPS_PlayerState* RPS_PlayerState);
};

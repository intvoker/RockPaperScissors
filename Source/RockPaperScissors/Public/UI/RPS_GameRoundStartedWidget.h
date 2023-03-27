// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPS_GameRoundStartedWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_GameRoundStartedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FText GetRoundInfo();
};

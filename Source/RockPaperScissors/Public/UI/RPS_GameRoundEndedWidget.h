// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPS_GameRoundEndedWidget.generated.h"

class ARPS_GameModeBase;
class ARPS_PlayerState;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_GameRoundEndedWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundResultTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundStatsTextBlock;

	virtual void NativeConstruct() override;

private:
	void HandleOnNativeVisibilityChanged(ESlateVisibility InVisibility);

	FText GetRoundResultInfo() const;

	FText GetRoundStatsInfo() const;

	static FString GetPlayerStateRoundStatsInfo(const ARPS_GameModeBase* RPS_GameModeBase,
	                                             const ARPS_PlayerState* RPS_PlayerState);
};

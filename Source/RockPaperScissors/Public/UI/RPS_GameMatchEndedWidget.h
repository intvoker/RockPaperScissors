// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPS_GameMatchEndedWidget.generated.h"

class ARPS_PlayerState;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_GameMatchEndedWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchResultTextBlock;

	virtual void NativeConstruct() override;

private:
	void HandleOnNativeVisibilityChanged(ESlateVisibility InVisibility);

	FText GetMatchResultInfo() const;

	static FString GetPlayerStateMatchResultInfo(const ARPS_PlayerState* RPS_PlayerState);
};

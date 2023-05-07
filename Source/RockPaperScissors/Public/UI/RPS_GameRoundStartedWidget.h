// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPS_GameRoundStartedWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_GameRoundStartedWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundNumberTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundRemainingSecondsTextBlock;

	virtual void NativeConstruct() override;

private:
	void HandleOnNativeVisibilityChanged(ESlateVisibility InVisibility);

	UFUNCTION()
	void HandleOnRoundRemainingSecondsChanged(int32 RoundRemainingSeconds);

	FText GetRoundNumberInfo() const;

	FText GetRoundRemainingSecondsInfo() const;
};

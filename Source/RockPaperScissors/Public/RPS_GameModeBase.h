// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPS_GameModeBase.generated.h"

class ARPS_Hand;
class ARPS_Pawn;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<ARPS_Pawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float RivalPawnDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	int32 NumberOfPlayingPoses = 3;

private:
	UPROPERTY()
	ARPS_Pawn* AIPawn;

	ARPS_Pawn* SpawnRivalPawn(ARPS_Pawn* Pawn, TSubclassOf<ARPS_Pawn> RivalPawnClass) const;

	UFUNCTION()
	void HandleOnHandPoseRecognizedLeft(int32 PoseIndex, FString PoseName);

	UFUNCTION()
	void HandleOnHandPoseRecognizedRight(int32 PoseIndex, FString PoseName);

	void SetHandPose(ARPS_Hand* Hand, int32 PoseIndex, FString PoseName) const;

	int32 GetWinHandPoseIndex(int32 PoseIndex) const;
};

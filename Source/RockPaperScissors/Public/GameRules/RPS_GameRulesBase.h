// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RPS_GameRulesBase.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_GameRulesBase : public AInfo
{
	GENERATED_BODY()

public:
	bool IsPlayingPose(int32 PoseIndex) const;

	virtual int32 GetRandomHandPoseIndex() const;

	virtual void GetWinHandPoseIndexes(int32 PoseIndex, TArray<int32>& OutPoseIndexes) const;

protected:
	virtual int32 GetNumberOfPoses() const;
};

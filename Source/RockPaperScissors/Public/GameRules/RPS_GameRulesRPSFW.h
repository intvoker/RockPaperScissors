// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameRules/RPS_GameRulesRPS.h"
#include "RPS_GameRulesRPSFW.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_GameRulesRPSFW : public ARPS_GameRulesRPS
{
	GENERATED_BODY()

public:
	virtual int32 GetRandomHandPoseIndex() const override;

	virtual void GetWinHandPoseIndexes(int32 PoseIndex, TArray<int32>& OutPoseIndexes) const override;

protected:
	const int32 FireIndex = 3;
	const int32 WaterIndex = 4;

	virtual int32 GetNumberOfPoses() const override;

private:
	const int32 NumberOfPoses = 5;
};

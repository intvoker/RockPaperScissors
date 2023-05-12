// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameRules/RPS_GameRulesBase.h"
#include "RPS_GameRulesRPS.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_GameRulesRPS : public ARPS_GameRulesBase
{
	GENERATED_BODY()

public:
	virtual int32 GetRandomHandPoseIndex() const override;

	virtual void GetWinHandPoseIndexes(int32 PoseIndex, TArray<int32>& OutPoseIndexes) const override;

protected:
	const int32 RockIndex = 0;
	const int32 PaperIndex = 1;
	const int32 ScissorsIndex = 2;

	virtual int32 GetNumberOfPoses() const override;

private:
	const int32 NumberOfPoses = 3;
};

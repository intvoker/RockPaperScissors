// Copyright intvoker. All Rights Reserved.


#include "GameRules/RPS_GameRulesBase.h"

#include "Hands/RPS_Hand.h"

bool ARPS_GameRulesBase::IsPlayingPose(int32 PoseIndex) const
{
	return PoseIndex >= 0 && PoseIndex < GetNumberOfPoses();
}

int32 ARPS_GameRulesBase::GetRandomHandPoseIndex() const
{
	return ARPS_Hand::DefaultHandPoseIndex;
}

void ARPS_GameRulesBase::GetWinHandPoseIndexes(int32 PoseIndex, TArray<int32>& OutPoseIndexes) const
{
}

int32 ARPS_GameRulesBase::GetNumberOfPoses() const
{
	return 0;
}

// Copyright intvoker. All Rights Reserved.


#include "GameRules/RPS_GameRulesRPSFW.h"

#include "Hands/RPS_Hand.h"

int32 ARPS_GameRulesRPSFW::GetRandomHandPoseIndex() const
{
	const auto RandomIndex = FMath::RandRange(0, 2);
	switch (RandomIndex)
	{
	case 0:
		return Super::GetRandomHandPoseIndex();
	case 1:
		return FireIndex;
	case 2:
		return WaterIndex;
	default:
		return ARPS_Hand::DefaultHandPoseIndex;
	}
}

void ARPS_GameRulesRPSFW::GetWinHandPoseIndexes(int32 PoseIndex, TArray<int32>& OutPoseIndexes) const
{
	if (PoseIndex < Super::GetNumberOfPoses())
	{
		Super::GetWinHandPoseIndexes(PoseIndex, OutPoseIndexes);
		OutPoseIndexes.Add(FireIndex);
	}
	if (PoseIndex == FireIndex)
	{
		OutPoseIndexes.Add(WaterIndex);
	}
	if (PoseIndex == WaterIndex)
	{
		OutPoseIndexes.Add(RockIndex);
		OutPoseIndexes.Add(PaperIndex);
		OutPoseIndexes.Add(ScissorsIndex);
	}
}

int32 ARPS_GameRulesRPSFW::GetNumberOfPoses() const
{
	return NumberOfPoses;
}

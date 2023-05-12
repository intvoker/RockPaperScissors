// Copyright intvoker. All Rights Reserved.


#include "GameRules/RPS_GameRulesRPS.h"

int32 ARPS_GameRulesRPS::GetRandomHandPoseIndex() const
{
	return FMath::RandRange(0, NumberOfPoses - 1);
}

void ARPS_GameRulesRPS::GetWinHandPoseIndexes(int32 PoseIndex, TArray<int32>& OutPoseIndexes) const
{
	auto WinPoseIndex = PoseIndex + 1;
	if (WinPoseIndex >= NumberOfPoses)
	{
		WinPoseIndex = 0;
	}
	OutPoseIndexes.Add(WinPoseIndex);
}

int32 ARPS_GameRulesRPS::GetNumberOfPoses() const
{
	return NumberOfPoses;
}

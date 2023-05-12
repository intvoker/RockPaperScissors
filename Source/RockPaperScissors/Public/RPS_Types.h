// Copyright intvoker. All Rights Reserved.

#pragma once

#include "RPS_Types.generated.h"

UENUM(BlueprintType)
enum class ERPS_GameRules : uint8
{
	RPS = 0,
	RPSFW
};

UENUM(BlueprintType)
enum class ERPS_GameMatchState : uint8
{
	None = 0,
	Started,
	Ended
};

UENUM(BlueprintType)
enum class ERPS_GameMatchResult : uint8
{
	None = 0,
	Win,
	Loss,
	Tie
};

UENUM(BlueprintType)
enum class ERPS_GameRoundState : uint8
{
	None = 0,
	Started,
	Ended
};

UENUM(BlueprintType)
enum class ERPS_GameRoundResult : uint8
{
	None = 0,
	Win,
	Loss,
	Tie
};

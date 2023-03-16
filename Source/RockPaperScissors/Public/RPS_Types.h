// Copyright intvoker. All Rights Reserved.

#pragma once

#include "RPS_Types.generated.h"

UENUM(BlueprintType)
enum class ERPS_GameMatchState : uint8
{
	None = 0,
	Started,
	Paused,
	Ended
};

UENUM(BlueprintType)
enum class ERPS_GameRoundState : uint8
{
	None = 0,
	Started,
	Ended
};

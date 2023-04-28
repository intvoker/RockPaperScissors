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

USTRUCT(BlueprintType)
struct FRPS_GameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 NumberOfRounds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RoundTime = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	bool bImmediatePlay = false;
};

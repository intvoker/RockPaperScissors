// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPS_Types.h"
#include "RPS_GameModeBase.generated.h"

class ARPS_Hand;
class ARPS_Pawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameMatchStateChangedSignature, ERPS_GameMatchState, GameMatchState);

USTRUCT(BlueprintType)
struct FRPS_GameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 NumberOfRounds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RoundTime = 3;
};

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPS_GameModeBase();

	FGameMatchStateChangedSignature OnGameMatchStateChanged;

	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<ARPS_Pawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float RivalPawnDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	int32 NumberOfPlayingPoses = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FRPS_GameData GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float UpdateRoundTime = 1.0f;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	ARPS_Pawn* AIPawn;

	ERPS_GameMatchState GameMatchState = ERPS_GameMatchState::None;

	int32 CurrentRoundIndex = 1;
	int32 CurrentRoundRemainingSeconds = 0;
	FTimerHandle UpdateRoundTimerHandle;

	void SetGameMatchState(ERPS_GameMatchState InGameMatchState);

	void SetupPawns();

	void StartRound();
	void UpdateRound();

	void GameOver();

	ARPS_Pawn* SpawnRivalPawn(ARPS_Pawn* Pawn, TSubclassOf<ARPS_Pawn> RivalPawnClass) const;

	UFUNCTION()
	void HandleOnHandPoseRecognizedLeft(int32 PoseIndex, const FString& PoseName);

	UFUNCTION()
	void HandleOnHandPoseRecognizedRight(int32 PoseIndex, const FString& PoseName);

	void SetHandPose(ARPS_Hand* Hand, int32 PoseIndex, const FString& PoseName) const;

	int32 GetWinHandPoseIndex(int32 PoseIndex) const;
};

// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPS_Types.h"
#include "RPS_GameModeBase.generated.h"

class ARPS_Hand;
class ARPS_Pawn;
class ARPS_PlayerState;
class ARPS_WidgetActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameMatchStateChangedSignature, ERPS_GameMatchState, GameMatchState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameRoundStateChangedSignature, ERPS_GameRoundState, GameRoundState);

USTRUCT(BlueprintType)
struct FRPS_GameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 NumberOfRounds = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RoundTime = 5;
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
	FGameRoundStateChangedSignature OnGameRoundStateChanged;

	virtual void StartPlay() override;

	void StartMatch();
	void EndMatch();

	ARPS_WidgetActor* GetWidgetActor() const { return WidgetActor; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<ARPS_WidgetActor> WidgetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float WidgetActorDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<ARPS_Pawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float AIPawnDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	int32 NumberOfPlayingPoses = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	int32 StartRoundPoseIndex = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FRPS_GameData GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float UpdateRoundTime = 1.0f;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool IsPlayingPose(int32 PoseIndex) const { return PoseIndex >= 0 && PoseIndex < NumberOfPlayingPoses; }
	bool IsStartRoundPose(int32 PoseIndex) const { return PoseIndex == StartRoundPoseIndex; }

private:
	UPROPERTY()
	ARPS_WidgetActor* WidgetActor;

	UPROPERTY()
	ARPS_Pawn* AIPawn;

	ERPS_GameMatchState GameMatchState = ERPS_GameMatchState::None;
	ERPS_GameRoundState GameRoundState = ERPS_GameRoundState::None;

	int32 CurrentRoundIndex = 0;
	int32 CurrentRoundRemainingSeconds = GameData.RoundTime;
	FTimerHandle UpdateRoundTimerHandle;

	void ResetCounters();

	void SetupPawns();

	void SetGameMatchState(ERPS_GameMatchState InGameMatchState);
	void SetGameRoundState(ERPS_GameRoundState InGameRoundState);

	void StartRound();
	void UpdateRound();
	void EndRound();

	static FTransform GetSpawnTransform(const ARPS_Pawn* Pawn, float Distance);

	ARPS_WidgetActor* SpawnWidgetActor(ARPS_Pawn* Pawn, TSubclassOf<ARPS_WidgetActor> InWidgetActorClass) const;

	ARPS_Pawn* SpawnAIPawn(ARPS_Pawn* Pawn, TSubclassOf<ARPS_Pawn> InAIPawnClass) const;

	static void ResetPawn(const ARPS_Pawn* Pawn);

	ARPS_PlayerState* GetPlayerState() const;
	ARPS_PlayerState* GetAIPlayerState() const;

	UFUNCTION()
	void HandleOnLeftHandPoseRecognized(int32 PoseIndex, const FString& PoseName);

	UFUNCTION()
	void HandleOnRightHandPoseRecognized(int32 PoseIndex, const FString& PoseName);

	void HandleOnHandPoseRecognized(ARPS_Hand* AIHand, int32 PoseIndex, const FString& PoseName);

	void SetHandPose(ARPS_Hand* AIHand, int32 PoseIndex, const FString& PoseName) const;

	int32 GetWinHandPoseIndex(int32 PoseIndex) const;

	void PrintString(const FString& InString) const;
	void PrintPlayerStates() const;
};

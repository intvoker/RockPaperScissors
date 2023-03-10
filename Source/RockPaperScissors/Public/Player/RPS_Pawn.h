// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OculusInputFunctionLibrary.h"
#include "RPS_Pawn.generated.h"

class ARPS_Hand;
class UCameraComponent;
class UMotionControllerComponent;

UCLASS()
class ROCKPAPERSCISSORS_API ARPS_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARPS_Pawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* LeftMotionControllerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* LeftChildActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* RightMotionControllerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* RightChildActorComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Hands")
	bool bSpawnRivalHands = false;

	UPROPERTY(EditDefaultsOnly, Category = "Hands", meta = (EditCondition = "bSpawnRivalHands"))
	float RivalHandsDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Hands", meta = (EditCondition = "bSpawnRivalHands"))
	TSubclassOf<ARPS_Hand> LeftRivalHandClass;

	UPROPERTY(EditDefaultsOnly, Category = "Hands", meta = (EditCondition = "bSpawnRivalHands"))
	TSubclassOf<ARPS_Hand> RightRivalHandClass;

	UPROPERTY(EditDefaultsOnly, Category = "Hands", meta = (EditCondition = "bSpawnRivalHands"))
	int32 NumberOfPlayingPoses = 3;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ARPS_Hand* LeftHand = nullptr;
	UPROPERTY()
	ARPS_Hand* RightHand = nullptr;
	UPROPERTY()
	ARPS_Hand* ActiveHand = nullptr;

	void SetActiveHand(ARPS_Hand* InActiveHand) { ActiveHand = InActiveHand; }
	void SetActiveLeftHand() { SetActiveHand(LeftHand); }
	void SetActiveRightHand() { SetActiveHand(RightHand); }

	UPROPERTY()
	ARPS_Hand* LeftRivalHand = nullptr;
	UPROPERTY()
	ARPS_Hand* RightRivalHand = nullptr;
	UPROPERTY()
	ARPS_Hand* ActiveRivalHand = nullptr;

	void SetActiveRivalHand(ARPS_Hand* InActiveRivalHand) { ActiveRivalHand = InActiveRivalHand; }
	void SetActiveLeftRivalHand() { SetActiveRivalHand(LeftRivalHand); }
	void SetActiveRightRivalHand() { SetActiveRivalHand(RightRivalHand); }

	void SetupHands();
	ARPS_Hand* SpawnRivalHand(EOculusHandType HandType, TSubclassOf<ARPS_Hand> HandClass) const;

	void LogLeftHand();
	void LogRightHand();

	void SetHandPose(int32 PoseIndex);
	void ClearHandPose();

	void SetSimulateHandPhysics(bool bEnabled);

	void ResetHands();

	UFUNCTION()
	void HandleOnHandPoseRecognizedLeft(int32 PoseIndex, FString PoseName);

	UFUNCTION()
	void HandleOnHandPoseRecognizedRight(int32 PoseIndex, FString PoseName);

	void SetHandPose(ARPS_Hand* Hand, int32 PoseIndex, FString PoseName) const;

	int32 GetWinHandPoseIndex(int32 PoseIndex) const;
};

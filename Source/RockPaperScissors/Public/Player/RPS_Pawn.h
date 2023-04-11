// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RPS_Pawn.generated.h"

class ARPS_Hand;

UCLASS()
class ROCKPAPERSCISSORS_API ARPS_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARPS_Pawn();

	ARPS_Hand* GetLeftHand() const { return LeftHand; }
	ARPS_Hand* GetRightHand() const { return RightHand; }

	FTransform virtual GetLeftHandRelativeTransform() const;
	FTransform virtual GetRightHandRelativeTransform() const;

	void SetSimulateHandsPhysics(bool bEnabled) const;
	void ResetHands();

	void SetRivalPawn(ARPS_Pawn* InRivalPawn) { RivalPawn = InRivalPawn; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* LeftChildActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* RightChildActorComponent;

	UPROPERTY()
	ARPS_Hand* LeftHand = nullptr;
	UPROPERTY()
	ARPS_Hand* RightHand = nullptr;
	UPROPERTY()
	ARPS_Hand* ActiveHand = nullptr;

	UPROPERTY()
	ARPS_Pawn* RivalPawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetActiveHand(ARPS_Hand* InActiveHand) { ActiveHand = InActiveHand; }
	void SetActiveLeftHand() { SetActiveHand(LeftHand); }
	void SetActiveRightHand() { SetActiveHand(RightHand); }

	virtual void SetupHands();

	void LogLeftHand();
	void LogRightHand();

	void SetHandPose(int32 PoseIndex);
	void ClearHandPose();

	void SetSimulateHandPhysics(bool bEnabled);

	void StartMatch();
};

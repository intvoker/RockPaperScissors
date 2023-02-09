// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OculusInputFunctionLibrary.h"
#include "RPS_Pawn.generated.h"

class ARPS_Hand;
class UCameraComponent;
class UHandPoseRecognizer;
class UMotionControllerComponent;
class UPoseableHandComponent;

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
	UPoseableHandComponent* LeftPoseableHandComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHandPoseRecognizer* LeftHandPoseRecognizer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* RightMotionControllerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPoseableHandComponent* RightPoseableHandComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHandPoseRecognizer* RightHandPoseRecognizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	ARPS_Hand* LeftRivalHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	ARPS_Hand* RightRivalHand;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetRivalHand(ARPS_Hand* RivalHandParam);

private:
	EOculusHandType ActiveHandType = EOculusHandType::HandLeft;
	void SetActiveHandType(EOculusHandType ActiveHandTypeParam) { ActiveHandType = ActiveHandTypeParam; }

	UPROPERTY()
	ARPS_Hand* ActiveRivalHand = nullptr;
	void SetActiveRivalHand(ARPS_Hand* ActiveRivalHandParam) { ActiveRivalHand = ActiveRivalHandParam; }
	void SetActiveLeftRivalHand() { SetActiveRivalHand(LeftRivalHand); }
	void SetActiveRightRivalHand() { SetActiveRivalHand(RightRivalHand); }

	UPoseableHandComponent* GetActivePoseableHandComponent() const;
	UHandPoseRecognizer* GetActiveHandPoseRecognizer() const;
	void SetHandPose(int32 PoseIndex);
	void ClearHandPose();

	void PrintRecognizedHandPose(UHandPoseRecognizer* HandPoseRecognizer) const;
	static FName HandNameFromType(EOculusHandType HandType);

	void EnableHandPhysics();
	void DisableHandPhysics();
};

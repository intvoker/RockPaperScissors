// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/RPS_Pawn.h"
#include "RPS_PlayerPawn.generated.h"

class UCameraComponent;
class UMotionControllerComponent;

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_PlayerPawn : public ARPS_Pawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARPS_PlayerPawn();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FTransform GetLeftHandRelativeTransform() const override;
	virtual FTransform GetRightHandRelativeTransform() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* LeftMotionControllerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* RightMotionControllerComponent;

	virtual void BeginPlay() override;

	virtual void SetupHands() override;
};

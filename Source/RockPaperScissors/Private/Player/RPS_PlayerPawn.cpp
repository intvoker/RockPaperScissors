// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_PlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Hands/RPS_Hand.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"

ARPS_PlayerPawn::ARPS_PlayerPawn()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());

	LeftMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(
		"LeftMotionControllerComponent");
	LeftMotionControllerComponent->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	LeftMotionControllerComponent->SetupAttachment(GetRootComponent());

	LeftChildActorComponent->SetupAttachment(LeftMotionControllerComponent);

	RightMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(
		"RightMotionControllerComponent");
	RightMotionControllerComponent->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	RightMotionControllerComponent->SetupAttachment(GetRootComponent());

	RightChildActorComponent->SetupAttachment(RightMotionControllerComponent);
}

void ARPS_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LogLeftHand", IE_Pressed, this, &ThisClass::LogLeftHand);
	PlayerInputComponent->BindAction("LogRightHand", IE_Pressed, this, &ThisClass::LogRightHand);

	PlayerInputComponent->BindAction("SetActiveLeftHand", IE_Pressed, this, &ThisClass::SetActiveLeftHand);
	PlayerInputComponent->BindAction("SetActiveRightHand", IE_Pressed, this, &ThisClass::SetActiveRightHand);

	DECLARE_DELEGATE_OneParam(FSetHandPoseSignature, int32);

	PlayerInputComponent->BindAction<FSetHandPoseSignature>("SetPose1", IE_Pressed, this, &ThisClass::SetHandPose, 0);
	PlayerInputComponent->BindAction("SetPose1", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetHandPoseSignature>("SetPose2", IE_Pressed, this, &ThisClass::SetHandPose, 1);
	PlayerInputComponent->BindAction("SetPose2", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetHandPoseSignature>("SetPose3", IE_Pressed, this, &ThisClass::SetHandPose, 2);
	PlayerInputComponent->BindAction("SetPose3", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetHandPoseSignature>("SetPose4", IE_Pressed, this, &ThisClass::SetHandPose, 3);
	PlayerInputComponent->BindAction("SetPose4", IE_Released, this, &ThisClass::ClearHandPose);

	DECLARE_DELEGATE_OneParam(FSetSimulateHandPhysicsSignature, bool);

	PlayerInputComponent->BindAction<FSetSimulateHandPhysicsSignature>("EnableHandPhysics", IE_Pressed, this,
	                                                                   &ThisClass::SetSimulateHandPhysics, true);
	PlayerInputComponent->BindAction<FSetSimulateHandPhysicsSignature>("DisableHandPhysics", IE_Pressed, this,
	                                                                   &ThisClass::SetSimulateHandPhysics, false);

	PlayerInputComponent->BindAction("ResetHands", IE_Pressed, this, &ThisClass::ResetHands);

	PlayerInputComponent->BindAction("StartMatch", IE_Pressed, this, &ThisClass::StartMatch);

	PlayerInputComponent->BindAction("SpawnItemActor", IE_Pressed, this, &ThisClass::SpawnItemActor);
}

FTransform ARPS_PlayerPawn::GetLeftHandRelativeTransform() const
{
	return LeftMotionControllerComponent->GetRelativeTransform();
}

FTransform ARPS_PlayerPawn::GetRightHandRelativeTransform() const
{
	return RightMotionControllerComponent->GetRelativeTransform();
}

void ARPS_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

void ARPS_PlayerPawn::SetupHands()
{
	Super::SetupHands();

	LeftHand->SetHasOwner(true);
	RightHand->SetHasOwner(true);
}

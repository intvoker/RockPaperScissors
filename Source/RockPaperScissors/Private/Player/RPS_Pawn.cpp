// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_Pawn.h"

#include "Camera/CameraComponent.h"
#include "Hands/RPS_Hand.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "OculusInputFunctionLibrary.h"
#include "XRMotionControllerBase.h"

// Sets default values
ARPS_Pawn::ARPS_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());

	LeftMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(
		"LeftMotionControllerComponent");
	LeftMotionControllerComponent->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	LeftMotionControllerComponent->SetupAttachment(GetRootComponent());

	LeftChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("LeftChildActorComponent");
	LeftChildActorComponent->SetupAttachment(LeftMotionControllerComponent);

	RightMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(
		"RightMotionControllerComponent");
	RightMotionControllerComponent->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	RightMotionControllerComponent->SetupAttachment(GetRootComponent());

	RightChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("RightChildActorComponent");
	RightChildActorComponent->SetupAttachment(RightMotionControllerComponent);
}

// Called every frame
void ARPS_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LeftRivalHand)
	{
		LeftRivalHand->SetHandRelativeTransform(LeftMotionControllerComponent->GetRelativeTransform());
		LeftRivalHand->CopyHandPose(LeftHand);
	}

	if (RightRivalHand)
	{
		RightRivalHand->SetHandRelativeTransform(RightMotionControllerComponent->GetRelativeTransform());
		RightRivalHand->CopyHandPose(RightHand);
	}
}

// Called to bind functionality to input
void ARPS_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LogLeftHand", IE_Pressed, this, &ThisClass::LogLeftHand);
	PlayerInputComponent->BindAction("LogRightHand", IE_Pressed, this, &ThisClass::LogRightHand);

	PlayerInputComponent->BindAction("SetActiveLeftHand", IE_Pressed, this, &ThisClass::SetActiveLeftHand);
	PlayerInputComponent->BindAction("SetActiveRightHand", IE_Pressed, this, &ThisClass::SetActiveRightHand);

	PlayerInputComponent->BindAction("SetActiveLeftRivalHand", IE_Pressed, this, &ThisClass::SetActiveLeftRivalHand);
	PlayerInputComponent->BindAction("SetActiveRightRivalHand", IE_Pressed, this, &ThisClass::SetActiveRightRivalHand);

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
}

// Called when the game starts or when spawned
void ARPS_Pawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	SetupHands();
}

void ARPS_Pawn::SetupHands()
{
	LeftHand = Cast<ARPS_Hand>(LeftChildActorComponent->GetChildActor());
	LeftHand->SetHasOwner(true);
	RightHand = Cast<ARPS_Hand>(RightChildActorComponent->GetChildActor());
	RightHand->SetHasOwner(true);

	if (bSpawnRivalHands)
	{
		LeftRivalHand = SpawnRivalHand(EOculusHandType::HandLeft, LeftRivalHandClass);
		RightRivalHand = SpawnRivalHand(EOculusHandType::HandRight, RightRivalHandClass);
	}
}

ARPS_Hand* ARPS_Pawn::SpawnRivalHand(EOculusHandType HandType, TSubclassOf<ARPS_Hand> HandClass)
{
	auto HandLocation = GetActorLocation();
	HandLocation += GetActorForwardVector() * RivalHandsDistance;

	auto HandRotation = GetActorRotation();
	HandRotation.Yaw = -180.0f;

	const auto HandTransform = FTransform(HandRotation, HandLocation);

	if (const auto Hand = GetWorld()->SpawnActorDeferred<ARPS_Hand>(HandClass, HandTransform))
	{
		Hand->SetHandType(HandType);
		Hand->SetHasOwner(false);
		Hand->FinishSpawning(HandTransform);

		return Hand;
	}

	return nullptr;
}

void ARPS_Pawn::LogLeftHand()
{
	LeftHand->LogHandPose();
}

void ARPS_Pawn::LogRightHand()
{
	RightHand->LogHandPose();
}

void ARPS_Pawn::SetHandPose(int32 PoseIndex)
{
	if (ActiveHand)
	{
		ActiveHand->SetHandPose(PoseIndex);
	}

	if (ActiveRivalHand)
	{
		ActiveRivalHand->SetHandPose(PoseIndex);
	}
}

void ARPS_Pawn::ClearHandPose()
{
	if (ActiveHand)
	{
		ActiveHand->ClearHandPose();
	}

	if (ActiveRivalHand)
	{
		ActiveRivalHand->ClearHandPose();
	}
}

void ARPS_Pawn::SetSimulateHandPhysics(bool bEnabled)
{
	if (ActiveHand)
	{
		ActiveHand->SetSimulateHandPhysics(bEnabled);
	}

	if (ActiveRivalHand)
	{
		ActiveRivalHand->SetSimulateHandPhysics(bEnabled);
	}
}

void ARPS_Pawn::ResetHands()
{
	ActiveHand = nullptr;
	ActiveRivalHand = nullptr;

	LeftHand->SetSimulateHandPhysics(false);
	RightHand->SetSimulateHandPhysics(false);

	if (LeftRivalHand)
	{
		LeftRivalHand->SetSimulateHandPhysics(false);
	}

	if (RightRivalHand)
	{
		RightRivalHand->SetSimulateHandPhysics(false);
	}
}

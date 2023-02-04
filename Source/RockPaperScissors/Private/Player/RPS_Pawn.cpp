// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_Pawn.h"

#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HandPoseRecognizer.h"
#include "MotionControllerComponent.h"
#include "OculusInputFunctionLibrary.h"
#include "PoseableHandComponent.h"
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

	LeftPoseableHandComponent = CreateDefaultSubobject<UPoseableHandComponent>("LeftPoseableHandComponent");
	LeftPoseableHandComponent->SkeletonType = EOculusHandType::HandLeft;
	LeftPoseableHandComponent->MeshType = EOculusHandType::HandLeft;
	LeftPoseableHandComponent->SetupAttachment(LeftMotionControllerComponent);

	LeftHandPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>("LeftHandPoseRecognizer");
	LeftHandPoseRecognizer->Side = EOculusHandType::HandLeft;
	LeftHandPoseRecognizer->SetupAttachment(LeftMotionControllerComponent);

	RightMotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>(
		"RightMotionControllerComponent");
	RightMotionControllerComponent->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	RightMotionControllerComponent->SetupAttachment(GetRootComponent());

	RightPoseableHandComponent = CreateDefaultSubobject<UPoseableHandComponent>("RightPoseableHandComponent");
	RightPoseableHandComponent->SkeletonType = EOculusHandType::HandRight;
	RightPoseableHandComponent->MeshType = EOculusHandType::HandRight;
	RightPoseableHandComponent->SetupAttachment(RightMotionControllerComponent);

	RightHandPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>("RightHandPoseRecognizer");
	RightHandPoseRecognizer->Side = EOculusHandType::HandRight;
	RightHandPoseRecognizer->SetupAttachment(RightMotionControllerComponent);
}

// Called when the game starts or when spawned
void ARPS_Pawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

// Called every frame
void ARPS_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPS_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

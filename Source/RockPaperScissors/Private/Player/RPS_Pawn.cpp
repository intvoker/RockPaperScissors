// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_Pawn.h"

#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HandPoseRecognizer.h"
#include "Kismet/KismetSystemLibrary.h"
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

// Called every frame
void ARPS_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintRecognizedHandPose(LeftHandPoseRecognizer);
	PrintRecognizedHandPose(RightHandPoseRecognizer);
}

// Called to bind functionality to input
void ARPS_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LogLeftHand", IE_Pressed, LeftHandPoseRecognizer,
	                                 &UHandPoseRecognizer::LogEncodedHandPose);
	PlayerInputComponent->BindAction("LogRightHand", IE_Pressed, RightHandPoseRecognizer,
	                                 &UHandPoseRecognizer::LogEncodedHandPose);

	DECLARE_DELEGATE_OneParam(FSetActiveHandSignature, EOculusHandType);

	PlayerInputComponent->BindAction<FSetActiveHandSignature>("SetActiveLeftHand", IE_Pressed, this,
	                                                          &ThisClass::SetActiveHand, EOculusHandType::HandLeft);
	PlayerInputComponent->BindAction<FSetActiveHandSignature>("SetActiveRightHand", IE_Pressed, this,
	                                                          &ThisClass::SetActiveHand, EOculusHandType::HandRight);

	DECLARE_DELEGATE_OneParam(FSetPoseSignature, int32);

	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose1", IE_Pressed, this, &ThisClass::SetHandPose, 0);
	PlayerInputComponent->BindAction("SetPose1", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose2", IE_Pressed, this, &ThisClass::SetHandPose, 1);
	PlayerInputComponent->BindAction("SetPose2", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose3", IE_Pressed, this, &ThisClass::SetHandPose, 2);
	PlayerInputComponent->BindAction("SetPose3", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose4", IE_Pressed, this, &ThisClass::SetHandPose, 3);
	PlayerInputComponent->BindAction("SetPose4", IE_Released, this, &ThisClass::ClearHandPose);
}

// Called when the game starts or when spawned
void ARPS_Pawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

UPoseableHandComponent* ARPS_Pawn::GetActivePoseableHandComponent() const
{
	switch (ActiveHand)
	{
	case EOculusHandType::HandLeft:
		return LeftPoseableHandComponent;
	case EOculusHandType::HandRight:
		return RightPoseableHandComponent;
	case EOculusHandType::None:
		break;
	default: ;
	}

	return nullptr;
}

UHandPoseRecognizer* ARPS_Pawn::GetActiveHandPoseRecognizer() const
{
	switch (ActiveHand)
	{
	case EOculusHandType::HandLeft:
		return LeftHandPoseRecognizer;
	case EOculusHandType::HandRight:
		return RightHandPoseRecognizer;
	case EOculusHandType::None:
		break;
	default: ;
	}

	return nullptr;
}

void ARPS_Pawn::SetHandPose(int32 PoseIndex)
{
	if (PoseIndex < 0 || PoseIndex >= GetActiveHandPoseRecognizer()->Poses.Num())
		return;

	const auto Pose = GetActiveHandPoseRecognizer()->Poses[PoseIndex];

	GetActivePoseableHandComponent()->SetPose(Pose.CustomEncodedPose);
}

void ARPS_Pawn::ClearHandPose()
{
	GetActivePoseableHandComponent()->ClearPose();
}

void ARPS_Pawn::PrintRecognizedHandPose(UHandPoseRecognizer* HandPoseRecognizer) const
{
	int Index;
	FString Name;
	float Duration;
	float Error;
	float Confidence;

	if (HandPoseRecognizer->GetRecognizedHandPose(Index, Name, Duration, Error, Confidence))
	{
		const auto Output = FString::Printf(TEXT("%s %s"), *HandNameFromType(HandPoseRecognizer->Side).ToString(),
		                                    *Name);
		UKismetSystemLibrary::PrintString(GetWorld(), Output, true, false);
	}
}

FName ARPS_Pawn::HandNameFromType(EOculusHandType HandType)
{
	switch (HandType)
	{
	case EOculusHandType::HandLeft:
		return FXRMotionControllerBase::LeftHandSourceId;
	case EOculusHandType::HandRight:
		return FXRMotionControllerBase::RightHandSourceId;
	case EOculusHandType::None:
		break;
	default: ;
	}

	return NAME_None;
}

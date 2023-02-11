// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_Pawn.h"

#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HandPoseRecognizer.h"
#include "Hands/RPS_Hand.h"
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
	LeftPoseableHandComponent->bInitializePhysics = true;
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
	RightPoseableHandComponent->bInitializePhysics = true;
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

	LeftRivalHand->CopyHandPose(LeftMotionControllerComponent->GetRelativeTransform(), LeftPoseableHandComponent);
	RightRivalHand->CopyHandPose(RightMotionControllerComponent->GetRelativeTransform(), RightPoseableHandComponent);
}

// Called to bind functionality to input
void ARPS_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LogLeftHand", IE_Pressed, LeftHandPoseRecognizer,
	                                 &UHandPoseRecognizer::LogEncodedHandPose);
	PlayerInputComponent->BindAction("LogRightHand", IE_Pressed, RightHandPoseRecognizer,
	                                 &UHandPoseRecognizer::LogEncodedHandPose);

	DECLARE_DELEGATE_OneParam(FSetActiveHandTypeSignature, EOculusHandType);

	PlayerInputComponent->BindAction<FSetActiveHandTypeSignature>("SetActiveLeftHand", IE_Pressed, this,
	                                                              &ThisClass::SetActiveHandType,
	                                                              EOculusHandType::HandLeft);
	PlayerInputComponent->BindAction<FSetActiveHandTypeSignature>("SetActiveRightHand", IE_Pressed, this,
	                                                              &ThisClass::SetActiveHandType,
	                                                              EOculusHandType::HandRight);

	PlayerInputComponent->BindAction("SetActiveLeftRivalHand", IE_Pressed, this,
	                                 &ThisClass::SetActiveLeftRivalHand);
	PlayerInputComponent->BindAction("SetActiveRightRivalHand", IE_Pressed, this,
	                                 &ThisClass::SetActiveRightRivalHand);

	DECLARE_DELEGATE_OneParam(FSetPoseSignature, int32);

	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose1", IE_Pressed, this, &ThisClass::SetHandPose, 0);
	PlayerInputComponent->BindAction("SetPose1", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose2", IE_Pressed, this, &ThisClass::SetHandPose, 1);
	PlayerInputComponent->BindAction("SetPose2", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose3", IE_Pressed, this, &ThisClass::SetHandPose, 2);
	PlayerInputComponent->BindAction("SetPose3", IE_Released, this, &ThisClass::ClearHandPose);
	PlayerInputComponent->BindAction<FSetPoseSignature>("SetPose4", IE_Pressed, this, &ThisClass::SetHandPose, 3);
	PlayerInputComponent->BindAction("SetPose4", IE_Released, this, &ThisClass::ClearHandPose);

	PlayerInputComponent->BindAction("EnableHandPhysics", IE_Pressed, this, &ThisClass::EnableHandPhysics);
	PlayerInputComponent->BindAction("DisableHandPhysics", IE_Pressed, this, &ThisClass::DisableHandPhysics);
}

// Called when the game starts or when spawned
void ARPS_Pawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	for (const auto RivalHand : TActorRange<ARPS_Hand>(GetWorld()))
	{
		SetRivalHand(RivalHand);
	}
}

void ARPS_Pawn::SetRivalHand(ARPS_Hand* InRivalHand)
{
	switch (InRivalHand->GetHandType())
	{
	case EOculusHandType::HandLeft:
		LeftRivalHand = InRivalHand;
		break;
	case EOculusHandType::HandRight:
		RightRivalHand = InRivalHand;
		break;
	case EOculusHandType::None:
		break;
	default: ;
	}
}

UPoseableHandComponent* ARPS_Pawn::GetActivePoseableHandComponent() const
{
	switch (ActiveHandType)
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
	switch (ActiveHandType)
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

	//GetActivePoseableHandComponent()->SetPose(Pose.CustomEncodedPose);

	if (ActiveRivalHand)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ActiveRivalHand: %s."), *ActiveRivalHand->GetName());

		ActiveRivalHand->SetHandPose(Pose.CustomEncodedPose);
	}
}

void ARPS_Pawn::ClearHandPose()
{
	//GetActivePoseableHandComponent()->ClearPose();

	if (ActiveRivalHand)
	{
		ActiveRivalHand->ClearHandPose();
	}
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

void ARPS_Pawn::EnableHandPhysics()
{
	LeftRivalHand->SetSimulateHandPhysics(true);
	RightRivalHand->SetSimulateHandPhysics(true);
}

void ARPS_Pawn::DisableHandPhysics()
{
	LeftRivalHand->SetSimulateHandPhysics(false);
	RightRivalHand->SetSimulateHandPhysics(false);
}

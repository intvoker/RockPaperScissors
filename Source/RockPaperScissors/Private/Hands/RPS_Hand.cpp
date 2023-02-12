// Copyright intvoker. All Rights Reserved.


#include "Hands/RPS_Hand.h"

#include "HandPoseRecognizer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PoseableHandComponent.h"
#include "XRMotionControllerBase.h"

// Sets default values
ARPS_Hand::ARPS_Hand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	PoseableHandComponent = CreateDefaultSubobject<UPoseableHandComponent>("PoseableHandComponent");
	PoseableHandComponent->bInitializePhysics = true;
	PoseableHandComponent->SetupAttachment(GetRootComponent());

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetEnableGravity(false);
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	HandPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>("HandPoseRecognizer");
	HandPoseRecognizer->SetupAttachment(GetRootComponent());

	PostSetHandType(HandType);

	SetSimulateHandPhysics(false);
}

// Called every frame
void ARPS_Hand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintRecognizedHandPose();
}

void ARPS_Hand::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const auto PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARPS_Hand, HandType))
	{
		PostSetHandType(HandType);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ARPS_Hand::PrintRecognizedHandPose() const
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

void ARPS_Hand::SetHandPose(int32 PoseIndex)
{
	if (PoseIndex < 0 || PoseIndex >= HandPoseRecognizer->Poses.Num())
		return;

	const auto Pose = HandPoseRecognizer->Poses[PoseIndex];

	SetHandPose(Pose.CustomEncodedPose);
}

void ARPS_Hand::SetHandPose(FString PoseString)
{
	SkeletalMeshComponent->SetMasterPoseComponent(PoseableHandComponent);
	PoseableHandComponent->SetPose(PoseString);

	bActiveHandPose = true;
}

void ARPS_Hand::ClearHandPose()
{
	SkeletalMeshComponent->SetMasterPoseComponent(nullptr);
	PoseableHandComponent->ClearPose();

	bActiveHandPose = false;
}

void ARPS_Hand::SetHandRelativeTransform(const FTransform RelativeTransform) const
{
	PoseableHandComponent->SetRelativeTransform(RelativeTransform);
	SkeletalMeshComponent->SetRelativeTransform(RelativeTransform);
}

void ARPS_Hand::CopyHandPose(const UPoseableHandComponent* SourcePHC) const
{
	if (bActiveHandPose)
		return;

	FQuat RootBoneRotation = SkeletalMeshComponent->GetRelativeRotation().Quaternion();
	RootBoneRotation *= FixupRotation;
	RootBoneRotation.Normalize();
	SkeletalMeshComponent->SetRelativeRotation(RootBoneRotation);

	if (PoseableHandComponent->bSkeletalMeshInitialized && SourcePHC->bSkeletalMeshInitialized
		&& PoseableHandComponent->BoneSpaceTransforms.Num() == SourcePHC->BoneSpaceTransforms.Num())
	{
		PoseableHandComponent->BoneSpaceTransforms = SourcePHC->BoneSpaceTransforms;
	}
	PoseableHandComponent->MarkRefreshTransformDirty();
}

void ARPS_Hand::SetSimulateHandPhysics(bool bEnabled)
{
	bSimulateHandPhysics = bEnabled;

	//Disable/Enable PoseableHandComponent
	const auto CollisionProfilePHC = bEnabled
		? UCollisionProfile::NoCollision_ProfileName
		: UCollisionProfile::PhysicsActor_ProfileName;
	const auto CollisionEnabledPHC = bEnabled
		? ECollisionEnabled::NoCollision
		: ECollisionEnabled::QueryAndPhysics;

	//UE_LOG(LogTemp, Warning, TEXT("CollisionProfilePHC: %s."), *CollisionProfilePHC.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("CollisionEnabledPHC: %s."), *UEnum::GetValueAsString(CollisionEnabledPHC));

	for (const auto CollisionCapsule : PoseableHandComponent->CollisionCapsules)
	{
		CollisionCapsule.Capsule->SetCollisionProfileName(CollisionProfilePHC);
		CollisionCapsule.Capsule->SetCollisionEnabled(CollisionEnabledPHC);
	}

	PoseableHandComponent->SetVisibility(!bEnabled);

	//Enable/Disable SkeletalMeshComponent
	SkeletalMeshComponent->SetSimulatePhysics(bEnabled);

	const auto CollisionProfileSMC = bEnabled
		? UCollisionProfile::PhysicsActor_ProfileName
		: UCollisionProfile::NoCollision_ProfileName;
	const auto CollisionEnabledSMC = bEnabled
		? ECollisionEnabled::QueryAndPhysics
		: ECollisionEnabled::NoCollision;

	//UE_LOG(LogTemp, Warning, TEXT("CollisionProfileSMC: %s."), *CollisionProfileSMC.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("CollisionEnabledSMC: %s."), *UEnum::GetValueAsString(CollisionEnabledSMC));

	SkeletalMeshComponent->SetCollisionProfileName(CollisionProfileSMC);
	SkeletalMeshComponent->SetCollisionEnabled(CollisionEnabledSMC);

	SkeletalMeshComponent->SetVisibility(bEnabled);
}

// Called when the game starts or when spawned
void ARPS_Hand::BeginPlay()
{
	Super::BeginPlay();
}

void ARPS_Hand::PostSetHandType(EOculusHandType InHandType) const
{
	PoseableHandComponent->SkeletonType = InHandType;
	PoseableHandComponent->MeshType = InHandType;
	HandPoseRecognizer->Side = InHandType;
}

FName ARPS_Hand::HandNameFromType(EOculusHandType HandType)
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

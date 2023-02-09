// Copyright intvoker. All Rights Reserved.


#include "Hands/RPS_Hand.h"

#include "PoseableHandComponent.h"

// Sets default values
ARPS_Hand::ARPS_Hand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	PoseableHandComponent = CreateDefaultSubobject<UPoseableHandComponent>("PoseableHandComponent");
	PostSetHandType(HandType);
	PoseableHandComponent->bInitializePhysics = true;
	PoseableHandComponent->SetupAttachment(GetRootComponent());

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetEnableGravity(false);
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	SetSimulateHandPhysics(false);
}

// Called every frame
void ARPS_Hand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void ARPS_Hand::SetHandPose(FString PoseString)
{
	SkeletalMeshComponent->SetMasterPoseComponent(PoseableHandComponent);
	//PoseableHandComponent->SetPose(PoseString);

	bActiveHandPose = true;
}

void ARPS_Hand::ClearHandPose()
{
	SkeletalMeshComponent->SetMasterPoseComponent(nullptr);
	//PoseableHandComponent->ClearPose();

	bActiveHandPose = false;
}

void ARPS_Hand::SetSimulateHandPhysics(bool bEnabled)
{
	bHandPhysics = bEnabled;
	
	//Disable/Enable PoseableHandComponent
	const auto CollisionEnabledPHC = bEnabled
		? ECollisionEnabled::NoCollision
		: ECollisionEnabled::QueryAndPhysics;

	for (const auto CollisionCapsule : PoseableHandComponent->CollisionCapsules)
	{
		CollisionCapsule.Capsule->SetCollisionEnabled(CollisionEnabledPHC);
	}
	//PoseableHandComponent->SetVisibility(!bEnabled);

	//Enable/Disable SkeletalMeshComponent
	const auto CollisionEnabledSMC = bEnabled
		? ECollisionEnabled::QueryAndPhysics
		: ECollisionEnabled::NoCollision;
	const auto CollisionProfileSMC = bEnabled
		? UCollisionProfile::PhysicsActor_ProfileName
		: UCollisionProfile::NoCollision_ProfileName;

	SkeletalMeshComponent->SetSimulatePhysics(bEnabled);
	SkeletalMeshComponent->SetCollisionEnabled(CollisionEnabledSMC);
	SkeletalMeshComponent->SetCollisionProfileName(CollisionProfileSMC);
	//SkeletalMeshComponent->SetVisibility(bEnabled);
}

void ARPS_Hand::PostSetHandType(EOculusHandType HandTypeParam) const
{
	PoseableHandComponent->SkeletonType = HandTypeParam;
	PoseableHandComponent->MeshType = HandTypeParam;
}

// Called when the game starts or when spawned
void ARPS_Hand::BeginPlay()
{
	Super::BeginPlay();

	UOculusInputFunctionLibrary::InitializeHandPhysics(PoseableHandComponent->SkeletonType, PoseableHandComponent);
}

void ARPS_Hand::CopyHandPose(const FTransform RelativeTransform, const UPoseableHandComponent* PHC) const
{
	PoseableHandComponent->SetRelativeTransform(RelativeTransform);

	if (!bHandPhysics)
	{
		SkeletalMeshComponent->SetRelativeTransform(RelativeTransform);

		FQuat RootBoneRotation = SkeletalMeshComponent->GetRelativeRotation().Quaternion();
		RootBoneRotation *= FixupRotation;
		RootBoneRotation.Normalize();
		SkeletalMeshComponent->SetRelativeRotation(RootBoneRotation);
	}

	if (IsActiveHandPose())
		return;

	if (PHC->bSkeletalMeshInitialized && PoseableHandComponent->bSkeletalMeshInitialized)
	{
		PoseableHandComponent->BoneSpaceTransforms = PHC->BoneSpaceTransforms;
	}
	PoseableHandComponent->MarkRefreshTransformDirty();
}

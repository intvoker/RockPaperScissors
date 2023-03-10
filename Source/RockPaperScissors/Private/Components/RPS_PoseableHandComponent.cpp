// Copyright intvoker. All Rights Reserved.


#include "Components/RPS_PoseableHandComponent.h"

void URPS_PoseableHandComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bCustomPoseableHandMesh && bInitializePhysics)
	{
		AttachCollisionCapsules();
	}
}

// Hackish way to attach CollisionCapsules generated for ovrpSkeleton2 to current SkeletalMesh (works with OculusHand SkeletalMesh)
void URPS_PoseableHandComponent::AttachCollisionCapsules()
{
	// Saving current SkeletalMesh
	const auto SavedSkeletalMesh = SkeletalMesh;

	// Generating CollisionCapsules
	const auto TempSkeletalMesh = NewObject<USkeletalMesh>(this, TEXT("OculusHandMesh"));
	if (TempSkeletalMesh)
	{
		if (UOculusInputFunctionLibrary::GetHandSkeletalMesh(TempSkeletalMesh, SkeletonType, MeshType))
		{
			SetSkeletalMesh(TempSkeletalMesh);

			CollisionCapsules = UOculusInputFunctionLibrary::InitializeHandPhysics(SkeletonType, this);
		}
	}

	// Restoring current SkeletalMesh
	SetSkeletalMesh(SavedSkeletalMesh);

	// Attaching CollisionCapsules to current SkeletalMesh
	for (const auto CollisionCapsule : CollisionCapsules)
	{
		if (!BoneNameMappings.Contains(CollisionCapsule.BoneId))
			continue;

		const auto BoneName = BoneNameMappings[CollisionCapsule.BoneId];
		if (BoneName.IsNone())
			continue;

		CollisionCapsule.Capsule->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform, BoneName);
	}
}

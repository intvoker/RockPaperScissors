// Copyright (c) Meta Platforms, Inc. and affiliates.

#include "PoseableHandComponent.h"
#include "OculusHandPoseRecognitionModule.h"

void UPoseableHandComponent::BeginPlay()
{
	// Unfortunately, bCustomHandMesh in UOculusHandComponent is private
	if (SkeletalMesh)
	{
		bCustomHandMesh = true;
	}

	if (bCustomHandMesh && bInitializePhysics)
	{
		AttachCollisionCapsules();
	}

	Super::BeginPlay();
}

void UPoseableHandComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CustomPoseWeightCurrent != CustomPoseWeightTarget)
	{
		CustomPoseWeightCurrent = FMath::FInterpConstantTo(CustomPoseWeightCurrent, CustomPoseWeightTarget, DeltaTime, CustomPoseWeightLerpSpeed);
	}

	if (CustomPoseWeightCurrent > 0.f)
	{
		UpdateBonePose(EBone::Thumb_0, Thumb_0);
		UpdateBonePose(EBone::Thumb_1, Thumb_1);
		UpdateBonePose(EBone::Thumb_2, Thumb_2);
		UpdateBonePose(EBone::Thumb_3, Thumb_3);

		UpdateBonePose(EBone::Index_1, Index_1);
		UpdateBonePose(EBone::Index_2, Index_2);
		UpdateBonePose(EBone::Index_3, Index_3);

		UpdateBonePose(EBone::Middle_1, Middle_1);
		UpdateBonePose(EBone::Middle_2, Middle_2);
		UpdateBonePose(EBone::Middle_3, Middle_3);

		UpdateBonePose(EBone::Ring_1, Ring_1);
		UpdateBonePose(EBone::Ring_2, Ring_2);
		UpdateBonePose(EBone::Ring_3, Ring_3);

		UpdateBonePose(EBone::Pinky_0, Pinky_0);
		UpdateBonePose(EBone::Pinky_1, Pinky_1);
		UpdateBonePose(EBone::Pinky_2, Pinky_2);
		UpdateBonePose(EBone::Pinky_3, Pinky_3);
	}
}

// Hackish way to attach CollisionCapsules generated for ovrpSkeleton2 to current SkeletalMesh (works with OculusHand SkeletalMesh)
void UPoseableHandComponent::AttachCollisionCapsules()
{
	//saving current SkeletalMesh
	const auto SavedSkeletalMesh = SkeletalMesh;

	//generating CollisionCapsules
	const auto TempSkeletalMesh = NewObject<USkeletalMesh>(this, TEXT("OculusHandMesh"));
	if (TempSkeletalMesh)
	{
		if (UOculusInputFunctionLibrary::GetHandSkeletalMesh(TempSkeletalMesh, SkeletonType, MeshType))
		{
			SetSkeletalMesh(TempSkeletalMesh);

			CollisionCapsules = UOculusInputFunctionLibrary::InitializeHandPhysics(SkeletonType, this);
		}
	}

	//restoring current SkeletalMesh
	SetSkeletalMesh(SavedSkeletalMesh);

	//attaching CollisionCapsules to current SkeletalMesh
	for (const auto CollisionCapsule : CollisionCapsules)
	{
		if (!BoneNameMappings.Contains(CollisionCapsule.BoneId))
			continue;

		const auto BoneName = BoneNameMappings[CollisionCapsule.BoneId];

		if (BoneName.IsNone())
			continue;

		const auto SavedRelativeTransform = CollisionCapsule.Capsule->GetRelativeTransform();
		CollisionCapsule.Capsule->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale, BoneName);
		CollisionCapsule.Capsule->SetRelativeTransform(SavedRelativeTransform);
	}
}

void UPoseableHandComponent::UpdateBonePose(EBone Bone, ERecognizedBone PosedBone)
{
	auto BoneIndex = (int)Bone;

	if (bCustomHandMesh && BoneNameMappings.Contains(Bone))
	{
		const auto MappedBoneIndex = SkeletalMesh->GetRefSkeleton().FindBoneIndex(BoneNameMappings[Bone]);
		if (MappedBoneIndex != INDEX_NONE)
		{
			BoneIndex = MappedBoneIndex;
		}
	}

	if (BoneSpaceTransforms.Num() > BoneIndex)
	{
		auto const TrackedRotation = BoneSpaceTransforms[BoneIndex].GetRotation();
		auto const PosedRotation = CustomPose.GetRotator(PosedBone).Quaternion();
		auto const PosedWeight = CustomPose.GetWeight(PosedBone) * CustomPoseWeightCurrent;

		if (PosedWeight > 0)
		{
			auto const NewRotation = FQuat::Slerp(TrackedRotation, PosedRotation, PosedWeight);
			BoneSpaceTransforms[BoneIndex].SetRotation(NewRotation);
		}
	}
}


void UPoseableHandComponent::SetPose(FString PoseString, float LerpSpeedOverride)
{
	CustomPose.CustomEncodedPose = PoseString;


	if (CustomPose.Decode())
	{
		CustomPoseWeightTarget = 1.f;
		CustomPoseWeightCurrent = 0.f;
		CustomPoseWeightLerpSpeed = LerpSpeedOverride > 0 ? LerpSpeedOverride : DefaultLerpSpeed;
	}
	else
	{
		UE_LOG(LogHandPoseRecognition, Warning, TEXT("Pose string '%s' can not be decoded."), *PoseString);
	}
}

void UPoseableHandComponent::ClearPose(float LerpSpeedOverride)
{
	CustomPoseWeightTarget = 0.f;
	CustomPoseWeightLerpSpeed = LerpSpeedOverride > 0 ? LerpSpeedOverride : DefaultLerpSpeed;
}

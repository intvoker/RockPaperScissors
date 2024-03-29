// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OculusInputFunctionLibrary.h"
#include "RPS_Hand.generated.h"

class UHandPoseRecognizer;
class UPoseableHandComponent;
struct FHandPose;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandPoseRecognizedSignature, int32, PoseIndex, const FString&, PoseName);

UCLASS()
class ROCKPAPERSCISSORS_API ARPS_Hand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPS_Hand();

	static const int32 DefaultHandPoseIndex;
	static const FString DefaultHandPoseName;

	FHandPoseRecognizedSignature OnHandPoseRecognized;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPoseableHandComponent* GetPoseableHandComponent() const { return PoseableHandComponent; }

	void SetHandType(EOculusHandType InHandType);

	void SetHasOwner(bool bInHasOwner);

	virtual bool HasLocalNetOwner() const override;

	void LogHandPose() const;

	FString GetHandPoseName(int32 PoseIndex) const;

	int32 GetHandPose() const { return CurrentHandPoseIndex; }
	void SetHandPose(int32 PoseIndex);
	void SetHandPose(const FString& PoseString);
	void ClearHandPose();

	void SetHandRelativeTransform(const FTransform& RelativeTransform) const;
	void CopyHandPose(const ARPS_Hand* SourceHand) const;

	void SetSimulateHandPhysics(bool bEnabled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPoseableHandComponent* PoseableHandComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHandPoseRecognizer* HandPoseRecognizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	EOculusHandType HandType = EOculusHandType::HandLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	bool bPrintRecognizedHandPose = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateSkeletalMeshComponentTransform() const;

	void RecognizeHandPose();

	void PostSetHandType(EOculusHandType InHandType) const;

	FHandPose* FindHandPose(int32 PoseIndex) const;

	void PrintHandPose(EOculusHandType Side, const FString& Name) const;
	static FName HandNameFromType(EOculusHandType HandType);

private:
	bool bHasOwner = false;

	bool bActiveHandPose = false;

	bool bSimulateHandPhysics = false;

	FQuat HandRootFixupRotation = FQuat(0.0f, 0.0f, 0.5f, 0.5f);

	int32 CurrentHandPoseIndex = DefaultHandPoseIndex;
	FString CurrentHandPoseName = DefaultHandPoseName;
};

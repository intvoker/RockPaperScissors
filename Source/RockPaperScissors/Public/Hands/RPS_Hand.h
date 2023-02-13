// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OculusInputFunctionLibrary.h"
#include "RPS_Hand.generated.h"

class UHandPoseRecognizer;
class UPoseableHandComponent;

UCLASS()
class ROCKPAPERSCISSORS_API ARPS_Hand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPS_Hand();

	bool GetHasOwner() const { return bHasOwner; }
	void SetHasOwner(bool bInHasOwner);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual bool HasLocalNetOwner() const override;

	UPoseableHandComponent* GetPoseableHandComponent() const { return PoseableHandComponent; }
	EOculusHandType GetHandType() const { return HandType; }

	void LogHandPose();
	void PrintRecognizedHandPose() const;
	void SetHandPose(int32 PoseIndex);
	void SetHandPose(FString PoseString);
	void ClearHandPose();

	void SetHandRelativeTransform(const FTransform RelativeTransform) const;
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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostSetHandType(EOculusHandType InHandType) const;

	static FName HandNameFromType(EOculusHandType HandType);

	void UpdateSkeletalMeshComponentTransform() const;

private:
	bool bHasOwner = false;

	bool bActiveHandPose = false;

	bool bSimulateHandPhysics = false;

	FQuat HandRootFixupRotation = FQuat(0.0f, 0.0f, 0.5f, 0.5f);;
};

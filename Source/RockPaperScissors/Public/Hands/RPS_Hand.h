// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OculusInputFunctionLibrary.h"
#include "RPS_Hand.generated.h"

class UPoseableHandComponent;

UCLASS()
class ROCKPAPERSCISSORS_API ARPS_Hand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPS_Hand();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	EOculusHandType GetHandType() const { return HandType; }

	bool IsActiveHandPose() const { return bActiveHandPose; }

	void SetHandPose(FString PoseString);
	void ClearHandPose();
	void CopyHandPose(FTransform RelativeTransform, const UPoseableHandComponent* PHC) const;

	void SetSimulateHandPhysics(bool bEnabled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPoseableHandComponent* PoseableHandComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	EOculusHandType HandType = EOculusHandType::HandLeft;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostSetHandType(EOculusHandType HandTypeParam) const;

private:
	bool bActiveHandPose = false;

	bool bHandPhysics = false;

	FQuat FixupRotation = FQuat(0.0f, 0.0f, 0.5f, 0.5f);;
};

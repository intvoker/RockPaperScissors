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
	void SetHandPose(FString PoseString) const;
	void ClearHandPose() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPoseableHandComponent* PoseableHandComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
	EOculusHandType HandType = EOculusHandType::HandLeft;

	void PostSetHandType(EOculusHandType HandTypeParam) const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

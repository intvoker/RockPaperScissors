// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PoseableHandComponent.h"
#include "RPS_PoseableHandComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API URPS_PoseableHandComponent : public UPoseableHandComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	void AttachCollisionCapsules();

};

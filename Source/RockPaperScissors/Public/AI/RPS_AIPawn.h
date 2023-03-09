// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/RPS_Pawn.h"
#include "RPS_AIPawn.generated.h"

/**
 * 
 */
UCLASS()
class ROCKPAPERSCISSORS_API ARPS_AIPawn : public ARPS_Pawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARPS_AIPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupHands() override;
};

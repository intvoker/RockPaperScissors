// Copyright intvoker. All Rights Reserved.


#include "AI/RPS_AIPawn.h"

#include "Hands/RPS_Hand.h"
#include "Player/RPS_PlayerPawn.h"

ARPS_AIPawn::ARPS_AIPawn()
{

}

void ARPS_AIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!RivalPawn)
		return;

	LeftHand->SetHandRelativeTransform(RivalPawn->GetLeftHandRelativeTransform());
	LeftHand->CopyHandPose(RivalPawn->GetLeftHand());

	RightHand->SetHandRelativeTransform(RivalPawn->GetRightHandRelativeTransform());
	RightHand->CopyHandPose(RivalPawn->GetRightHand());
}

void ARPS_AIPawn::SetupHands()
{
	Super::SetupHands();

	LeftHand->SetHasOwner(false);
	RightHand->SetHasOwner(false);
}

// Copyright intvoker. All Rights Reserved.


#include "RPS_GameModeBase.h"

#include "Hands/RPS_Hand.h"
#include "Player/RPS_PlayerPawn.h"

void ARPS_GameModeBase::StartPlay()
{
	Super::StartPlay();

	const auto PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController)
		return;

	const auto Pawn = Cast<ARPS_Pawn>(PlayerController->GetPawnOrSpectator());
	if (!Pawn)
		return;

	if (const auto LeftHand = Pawn->GetLeftHand())
	{
		LeftHand->OnHandPoseRecognized.AddDynamic(this, &ThisClass::HandleOnHandPoseRecognizedLeft);
	}
	if (const auto RightHand = Pawn->GetRightHand())
	{
		RightHand->OnHandPoseRecognized.AddDynamic(this, &ThisClass::HandleOnHandPoseRecognizedRight);
	}

	AIPawn = SpawnRivalPawn(Pawn, AIPawnClass);
}

ARPS_Pawn* ARPS_GameModeBase::SpawnRivalPawn(ARPS_Pawn* Pawn, TSubclassOf<ARPS_Pawn> RivalPawnClass) const
{
	auto RivalPawnLocation = Pawn->GetActorLocation();
	RivalPawnLocation += Pawn->GetActorForwardVector() * RivalPawnDistance;

	auto RivalPawnRotation = Pawn->GetActorRotation();
	RivalPawnRotation.Yaw = -180.0f;

	const auto RivalPawnTransform = FTransform(RivalPawnRotation, RivalPawnLocation);

	if (const auto RivalPawn = GetWorld()->SpawnActorDeferred<ARPS_Pawn>(RivalPawnClass, RivalPawnTransform))
	{
		RivalPawn->SetRivalPawn(Pawn);
		RivalPawn->FinishSpawning(RivalPawnTransform);

		return RivalPawn;
	}

	return nullptr;
}

void ARPS_GameModeBase::HandleOnHandPoseRecognizedLeft(int32 PoseIndex, const FString& PoseName)
{
	if (!AIPawn)
		return;

	if (const auto AILeftHand = AIPawn->GetLeftHand())
	{
		SetHandPose(AILeftHand, PoseIndex, PoseName);
	}
}

void ARPS_GameModeBase::HandleOnHandPoseRecognizedRight(int32 PoseIndex, const FString& PoseName)
{
	if (!AIPawn)
		return;

	if (const auto AIRightHand = AIPawn->GetRightHand())
	{
		SetHandPose(AIRightHand, PoseIndex, PoseName);
	}
}

void ARPS_GameModeBase::SetHandPose(ARPS_Hand* Hand, int32 PoseIndex, const FString& PoseName) const
{
	if (!Hand)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("SetHandPose: %s %d %s."), *Hand->GetName(), PoseIndex, *PoseName);

	const auto WinPoseIndex = GetWinHandPoseIndex(PoseIndex);

	if (WinPoseIndex != ARPS_Hand::DefaultHandPoseIndex)
	{
		Hand->SetHandPose(WinPoseIndex);
	}
	else
	{
		Hand->ClearHandPose();
	}
}

int32 ARPS_GameModeBase::GetWinHandPoseIndex(int32 PoseIndex) const
{
	if (PoseIndex == ARPS_Hand::DefaultHandPoseIndex)
		return ARPS_Hand::DefaultHandPoseIndex;

	if (PoseIndex < 0 || PoseIndex >= NumberOfPlayingPoses)
		return ARPS_Hand::DefaultHandPoseIndex;

	auto WinPoseIndex = PoseIndex + 1;
	if (WinPoseIndex >= NumberOfPlayingPoses)
	{
		WinPoseIndex = 0;
	}

	return WinPoseIndex;
}

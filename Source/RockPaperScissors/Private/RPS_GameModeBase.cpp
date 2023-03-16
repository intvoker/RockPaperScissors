// Copyright intvoker. All Rights Reserved.


#include "RPS_GameModeBase.h"

#include "Hands/RPS_Hand.h"
#include "Player/RPS_PlayerPawn.h"

ARPS_GameModeBase::ARPS_GameModeBase()
{
	DefaultPawnClass = ARPS_PlayerPawn::StaticClass();
}

void ARPS_GameModeBase::StartPlay()
{
	Super::StartPlay();

	SetupPawns();

	StartRound();

	SetGameMatchState(ERPS_GameMatchState::Started);
}

void ARPS_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPS_GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(UpdateRoundTimerHandle);
}

void ARPS_GameModeBase::SetGameMatchState(ERPS_GameMatchState InGameMatchState)
{
	if (GameMatchState == InGameMatchState)
		return;

	GameMatchState = InGameMatchState;

	OnGameMatchStateChanged.Broadcast(GameMatchState);
}

void ARPS_GameModeBase::SetupPawns()
{
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

void ARPS_GameModeBase::StartRound()
{
	CurrentRoundRemainingSeconds = GameData.RoundTime;

	GetWorld()->GetTimerManager().SetTimer(UpdateRoundTimerHandle, this, &ThisClass::UpdateRound, UpdateRoundTime,
	                                       true);
}

void ARPS_GameModeBase::UpdateRound()
{
	CurrentRoundRemainingSeconds -= UpdateRoundTime;

	if (CurrentRoundRemainingSeconds <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateRoundTimerHandle);

		if (CurrentRoundIndex < GameData.NumberOfRounds)
		{
			CurrentRoundIndex++;

			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ARPS_GameModeBase::GameOver()
{
	SetGameMatchState(ERPS_GameMatchState::Finished);
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

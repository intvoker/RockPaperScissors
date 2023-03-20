// Copyright intvoker. All Rights Reserved.


#include "RPS_GameModeBase.h"

#include "Hands/RPS_Hand.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/RPS_PlayerController.h"
#include "Player/RPS_PlayerPawn.h"
#include "Player/RPS_PlayerState.h"

ARPS_GameModeBase::ARPS_GameModeBase()
{
	PlayerControllerClass = ARPS_PlayerController::StaticClass();
	PlayerStateClass = ARPS_PlayerState::StaticClass();
	DefaultPawnClass = ARPS_PlayerPawn::StaticClass();
}

void ARPS_GameModeBase::StartPlay()
{
	Super::StartPlay();

	SetupPawns();

	StartMatch();
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
		LeftHand->OnHandPoseRecognized.AddDynamic(this, &ThisClass::HandleOnLeftHandPoseRecognized);
	}
	if (const auto RightHand = Pawn->GetRightHand())
	{
		RightHand->OnHandPoseRecognized.AddDynamic(this, &ThisClass::HandleOnRightHandPoseRecognized);
	}

	AIPawn = SpawnRivalPawn(Pawn, AIPawnClass);
}

void ARPS_GameModeBase::SetGameMatchState(ERPS_GameMatchState InGameMatchState)
{
	if (GameMatchState == InGameMatchState)
		return;

	GameMatchState = InGameMatchState;

	OnGameMatchStateChanged.Broadcast(GameMatchState);
}

void ARPS_GameModeBase::SetGameRoundState(ERPS_GameRoundState InGameRoundState)
{
	if (GameRoundState == InGameRoundState)
		return;

	GameRoundState = InGameRoundState;

	OnGameRoundStateChanged.Broadcast(GameRoundState);
}

void ARPS_GameModeBase::StartMatch()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("StartMatch"));

	SetGameMatchState(ERPS_GameMatchState::Started);
}

void ARPS_GameModeBase::EndMatch()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("EndMatch"));

	SetGameMatchState(ERPS_GameMatchState::Ended);
}

void ARPS_GameModeBase::StartRound()
{
	if (GameMatchState != ERPS_GameMatchState::Started)
		return;

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("StartRound"));

	SetGameRoundState(ERPS_GameRoundState::Started);

	CurrentRoundRemainingSeconds = GameData.RoundTime;

	GetWorld()->GetTimerManager().SetTimer(UpdateRoundTimerHandle, this, &ThisClass::UpdateRound, UpdateRoundTime,
	                                       true);
}

void ARPS_GameModeBase::UpdateRound()
{
	const auto Output = FString::Printf(
		TEXT("Round: %d / %d. Remaining %d seconds."), CurrentRoundIndex, GameData.NumberOfRounds,
		CurrentRoundRemainingSeconds);
	UKismetSystemLibrary::PrintString(GetWorld(), Output, true, true, FLinearColor::Green, 5.0);

	CurrentRoundRemainingSeconds -= UpdateRoundTime;

	if (CurrentRoundRemainingSeconds <= 0)
	{
		EndRound();
	}
}

void ARPS_GameModeBase::EndRound()
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("EndRound"));

	SetGameRoundState(ERPS_GameRoundState::Ended);

	GetWorld()->GetTimerManager().ClearTimer(UpdateRoundTimerHandle);

	if (CurrentRoundIndex < GameData.NumberOfRounds)
	{
		CurrentRoundIndex++;
	}
	else
	{
		EndMatch();
	}
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

void ARPS_GameModeBase::HandleOnLeftHandPoseRecognized(int32 PoseIndex, const FString& PoseName)
{
	if (!AIPawn)
		return;

	HandleOnHandPoseRecognized(AIPawn->GetLeftHand(), PoseIndex, PoseName);
}

void ARPS_GameModeBase::HandleOnRightHandPoseRecognized(int32 PoseIndex, const FString& PoseName)
{
	if (!AIPawn)
		return;

	HandleOnHandPoseRecognized(AIPawn->GetRightHand(), PoseIndex, PoseName);
}

void ARPS_GameModeBase::HandleOnHandPoseRecognized(ARPS_Hand* AIHand, int32 PoseIndex, const FString& PoseName)
{
	if (!AIHand)
		return;

	if (GameRoundState != ERPS_GameRoundState::Started && IsStartRoundPose(PoseIndex))
	{
		StartRound();
		return;
	}

	if (GameRoundState == ERPS_GameRoundState::Started && IsPlayingPose(PoseIndex))
	{
		SetHandPose(AIHand, PoseIndex, PoseName);
		EndRound();
	}
}

void ARPS_GameModeBase::SetHandPose(ARPS_Hand* AIHand, int32 PoseIndex, const FString& PoseName) const
{
	if (!AIHand)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("SetHandPose: %s %d %s."), *AIHand->GetName(), PoseIndex, *PoseName);

	const auto WinPoseIndex = GetWinHandPoseIndex(PoseIndex);

	if (WinPoseIndex != ARPS_Hand::DefaultHandPoseIndex)
	{
		AIHand->SetHandPose(WinPoseIndex);
	}
	else
	{
		AIHand->ClearHandPose();
	}
}

int32 ARPS_GameModeBase::GetWinHandPoseIndex(int32 PoseIndex) const
{
	if (PoseIndex == ARPS_Hand::DefaultHandPoseIndex)
		return ARPS_Hand::DefaultHandPoseIndex;

	if (!IsPlayingPose(PoseIndex))
		return ARPS_Hand::DefaultHandPoseIndex;

	auto WinPoseIndex = PoseIndex + 1;
	if (WinPoseIndex >= NumberOfPlayingPoses)
	{
		WinPoseIndex = 0;
	}

	return WinPoseIndex;
}

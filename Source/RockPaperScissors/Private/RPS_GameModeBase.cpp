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
	PrintString(TEXT("Start Match"));

	SetGameMatchState(ERPS_GameMatchState::Started);
}

void ARPS_GameModeBase::EndMatch()
{
	PrintString(TEXT("End Match"));

	SetGameMatchState(ERPS_GameMatchState::Ended);

	PrintPlayerStates();
}

void ARPS_GameModeBase::StartRound()
{
	if (GameMatchState != ERPS_GameMatchState::Started)
		return;

	CurrentRoundIndex++;
	CurrentRoundRemainingSeconds = GameData.RoundTime;

	PrintString(FString::Printf(TEXT("Start Round: %d"), CurrentRoundIndex));

	SetGameRoundState(ERPS_GameRoundState::Started);

	GetWorld()->GetTimerManager().SetTimer(UpdateRoundTimerHandle, this, &ThisClass::UpdateRound, UpdateRoundTime,
	                                       true);

	ResetPawn(AIPawn);
}

void ARPS_GameModeBase::UpdateRound()
{
	PrintString(FString::Printf(
		TEXT("Round: %d / %d. Remaining %d seconds."), CurrentRoundIndex, GameData.NumberOfRounds,
		CurrentRoundRemainingSeconds));

	CurrentRoundRemainingSeconds -= UpdateRoundTime;

	if (CurrentRoundRemainingSeconds <= 0)
	{
		EndRound();
	}
}

void ARPS_GameModeBase::EndRound()
{
	PrintString(FString::Printf(TEXT("End Round: %d"), CurrentRoundIndex));

	SetGameRoundState(ERPS_GameRoundState::Ended);

	GetWorld()->GetTimerManager().ClearTimer(UpdateRoundTimerHandle);

	if (CurrentRoundIndex >= GameData.NumberOfRounds)
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

void ARPS_GameModeBase::ResetPawn(const ARPS_Pawn* Pawn)
{
	if (!Pawn)
		return;

	if (const auto LeftHand = Pawn->GetLeftHand())
	{
		LeftHand->ClearHandPose();
	}
	if (const auto RightHand = Pawn->GetRightHand())
	{
		RightHand->ClearHandPose();
	}
}

ARPS_PlayerState* ARPS_GameModeBase::GetPlayerState() const
{
	const auto PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController)
		return nullptr;

	return PlayerController->GetPlayerState<ARPS_PlayerState>();
}

ARPS_PlayerState* ARPS_GameModeBase::GetAIPlayerState() const
{
	if (!AIPawn)
		return nullptr;

	return AIPawn->GetPlayerState<ARPS_PlayerState>();
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

	if (WinPoseIndex == ARPS_Hand::DefaultHandPoseIndex)
		return;

	AIHand->SetHandPose(WinPoseIndex);

	if (const auto PlayerState = GetPlayerState())
	{
		PlayerState->AddLoss();
	}
	if (const auto AIPlayerState = GetAIPlayerState())
	{
		AIPlayerState->AddWin();
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

void ARPS_GameModeBase::PrintString(const FString& InString) const
{
	UKismetSystemLibrary::PrintString(GetWorld(), InString, true, true, FLinearColor::Green, 5.0);
}

void ARPS_GameModeBase::PrintPlayerStates() const
{
	if (const auto PlayerState = GetPlayerState())
	{
		PrintString(FString::Printf(TEXT("Player Wins: %d"), PlayerState->GetWins()));
		PrintString(FString::Printf(TEXT("Player Losses: %d"), PlayerState->GetLosses()));
	}
	if (const auto AIPlayerState = GetAIPlayerState())
	{
		PrintString(FString::Printf(TEXT("AI Player Wins: %d"), AIPlayerState->GetWins()));
		PrintString(FString::Printf(TEXT("AI Player Losses: %d"), AIPlayerState->GetLosses()));
	}
}

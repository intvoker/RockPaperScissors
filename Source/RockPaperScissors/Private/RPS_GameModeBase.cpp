// Copyright intvoker. All Rights Reserved.


#include "RPS_GameModeBase.h"

#include "Hands/RPS_Hand.h"
#include "Player/RPS_PlayerController.h"
#include "Player/RPS_PlayerPawn.h"
#include "Player/RPS_PlayerState.h"
#include "UI/RPS_HUD.h"
#include "UI/RPS_WidgetActor.h"

ARPS_GameModeBase::ARPS_GameModeBase()
{
	PlayerControllerClass = ARPS_PlayerController::StaticClass();
	PlayerStateClass = ARPS_PlayerState::StaticClass();
	DefaultPawnClass = ARPS_PlayerPawn::StaticClass();
	HUDClass = ARPS_HUD::StaticClass();
}

void ARPS_GameModeBase::StartPlay()
{
	Super::StartPlay();

	SetupPawns();
	SetPlayerNames();

	StartMatch();
}

void ARPS_GameModeBase::StartMatch()
{
	SetGameMatchState(ERPS_GameMatchState::Started);

	ResetCounters();
	ResetPlayerStates();
	ResetPawns();
}

void ARPS_GameModeBase::EndMatch()
{
	Finished();
	Award();

	SetGameMatchState(ERPS_GameMatchState::Ended);

	ResetCounters();
}

ARPS_PlayerState* ARPS_GameModeBase::GetPlayerState() const
{
	if (!PlayerPawn)
		return nullptr;

	return PlayerPawn->GetPlayerState<ARPS_PlayerState>();
}

ARPS_PlayerState* ARPS_GameModeBase::GetAIPlayerState() const
{
	if (!AIPawn)
		return nullptr;

	return AIPawn->GetPlayerState<ARPS_PlayerState>();
}

FString ARPS_GameModeBase::GetHandPoseName(int32 PoseIndex) const
{
	if (!PlayerPawn)
		return ARPS_Hand::DefaultHandPoseName;

	const auto RightHand = PlayerPawn->GetRightHand();
	if (!RightHand)
		return ARPS_Hand::DefaultHandPoseName;

	return RightHand->GetHandPoseName(PoseIndex);
}

FString ARPS_GameModeBase::GetStartRoundHandPoseName() const
{
	return GetHandPoseName(StartRoundPoseIndex);
}

void ARPS_GameModeBase::SpawnItemActor() const
{
	if (!PlayerPawn)
		return;

	const auto Height = PlayerPawn->GetRightHandRelativeTransform().GetLocation().Z;

	const auto Transform = GetSpawnTransform(PlayerPawn, ItemActorDistance, Height);

	GetWorld()->SpawnActor<AActor>(ItemActorClass, Transform);
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

void ARPS_GameModeBase::ResetCounters()
{
	GameRoundState = ERPS_GameRoundState::None;

	CurrentRoundIndex = 0;
	CurrentRoundRemainingSeconds = GameData.RoundTime;
	GetWorld()->GetTimerManager().ClearTimer(UpdateRoundTimerHandle);
}

void ARPS_GameModeBase::ResetPlayerStates() const
{
	if (const auto PlayerState = GetPlayerState())
	{
		PlayerState->Reset();
	}
	if (const auto AIPlayerState = GetAIPlayerState())
	{
		AIPlayerState->Reset();
	}
}

void ARPS_GameModeBase::ResetPawns() const
{
	if (PlayerPawn)
	{
		PlayerPawn->ResetHands();
	}
	if (AIPawn)
	{
		AIPawn->ResetHands();
	}
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

	WidgetActor = SpawnWidgetActor(Pawn);

	AIPawn = SpawnAIPawn(Pawn);

	PlayerPawn = Pawn;
}

void ARPS_GameModeBase::SetPlayerNames() const
{
	if (const auto PlayerState = GetPlayerState())
	{
		PlayerState->SetPlayerName(TEXT("Player"));
	}
	if (const auto AIPlayerState = GetAIPlayerState())
	{
		AIPlayerState->SetPlayerName(TEXT("AI Player"));
	}
}

void ARPS_GameModeBase::SetGameMatchState(ERPS_GameMatchState InGameMatchState)
{
	GameMatchState = InGameMatchState;

	OnGameMatchStateChanged.Broadcast(GameMatchState);
}

void ARPS_GameModeBase::SetGameRoundState(ERPS_GameRoundState InGameRoundState)
{
	GameRoundState = InGameRoundState;

	OnGameRoundStateChanged.Broadcast(GameRoundState);
}

void ARPS_GameModeBase::StartRound()
{
	if (GameMatchState != ERPS_GameMatchState::Started)
		return;

	if (CurrentRoundIndex >= GameData.NumberOfRounds)
	{
		EndMatch();
		return;
	}

	CurrentRoundIndex++;
	CurrentRoundRemainingSeconds = GameData.RoundTime;

	SetGameRoundState(ERPS_GameRoundState::Started);

	GetWorld()->GetTimerManager().SetTimer(UpdateRoundTimerHandle, this, &ThisClass::UpdateRound, UpdateRoundTime,
	                                       true);

	ResetPawns();
}

void ARPS_GameModeBase::UpdateRound()
{
	CurrentRoundRemainingSeconds -= UpdateRoundTime;

	if (CurrentRoundRemainingSeconds <= 0)
	{
		ReadHandPoses();
		EndRound();
	}
}

void ARPS_GameModeBase::EndRound()
{
	SetGameRoundState(ERPS_GameRoundState::Ended);

	GetWorld()->GetTimerManager().ClearTimer(UpdateRoundTimerHandle);
}

FTransform ARPS_GameModeBase::GetSpawnTransform(const ARPS_Pawn* Pawn, const float Distance, const float Height)
{
	auto Location = Pawn->GetActorLocation();
	Location += Pawn->GetActorForwardVector() * Distance;
	Location += Pawn->GetActorUpVector() * Height;

	auto Rotation = Pawn->GetActorRotation();
	Rotation.Yaw = -180.0f;

	return FTransform(Rotation, Location);
}

ARPS_WidgetActor* ARPS_GameModeBase::SpawnWidgetActor(const ARPS_Pawn* Pawn) const
{
	const auto Transform = GetSpawnTransform(Pawn, WidgetActorDistance);

	return GetWorld()->SpawnActor<ARPS_WidgetActor>(WidgetActorClass, Transform);
}

ARPS_Pawn* ARPS_GameModeBase::SpawnAIPawn(ARPS_Pawn* Pawn) const
{
	const auto Transform = GetSpawnTransform(Pawn, AIPawnDistance);

	if (const auto SpawnedAIPawn = GetWorld()->SpawnActorDeferred<ARPS_Pawn>(AIPawnClass, Transform))
	{
		SpawnedAIPawn->SetRivalPawn(Pawn);
		SpawnedAIPawn->FinishSpawning(Transform);

		return SpawnedAIPawn;
	}

	return nullptr;
}

void ARPS_GameModeBase::HandleOnLeftHandPoseRecognized(int32 PoseIndex, const FString& PoseName)
{
	HandleOnHandPoseRecognized(PoseIndex, PoseName);
}

void ARPS_GameModeBase::HandleOnRightHandPoseRecognized(int32 PoseIndex, const FString& PoseName)
{
	HandleOnHandPoseRecognized(PoseIndex, PoseName);
}

void ARPS_GameModeBase::HandleOnHandPoseRecognized(int32 PoseIndex, const FString& PoseName)
{
	if (GameRoundState != ERPS_GameRoundState::Started && IsStartRoundPose(PoseIndex))
	{
		StartRound();
		return;
	}

	if (GameRoundState == ERPS_GameRoundState::Started && IsPlayingPose(PoseIndex) && GameData.bImmediatePlay)
	{
		ReadHandPoses();
		EndRound();
	}
}

void ARPS_GameModeBase::ReadHandPoses() const
{
	if (!PlayerPawn || !AIPawn)
		return;

	if (ReadHandPose(PlayerPawn->GetRightHand(), AIPawn->GetRightHand()))
		return;

	if (ReadHandPose(PlayerPawn->GetLeftHand(), AIPawn->GetLeftHand()))
		return;

	Posed(ARPS_Hand::DefaultHandPoseIndex, ARPS_Hand::DefaultHandPoseIndex);
}

bool ARPS_GameModeBase::ReadHandPose(const ARPS_Hand* PlayerHand, ARPS_Hand* AIHand) const
{
	if (!PlayerHand || !AIHand)
		return false;

	const auto PlayerPoseIndex = PlayerHand->GetHandPose();
	const auto AIPoseIndex = GetRandomHandPoseIndex();

	if (IsPlayingPose(PlayerPoseIndex))
	{
		AIHand->SetHandPose(AIPoseIndex);

		Posed(PlayerPoseIndex, AIPoseIndex);

		return true;
	}

	return false;
}

int32 ARPS_GameModeBase::GetRandomHandPoseIndex() const
{
	return FMath::RandRange(0, NumberOfPlayingPoses - 1);
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

void ARPS_GameModeBase::Posed(int32 PlayerPoseIndex, int32 AIPoseIndex) const
{
	const auto PlayerState = GetPlayerState();
	const auto AIPlayerState = GetAIPlayerState();

	if (!PlayerState || !AIPlayerState)
		return;

	if (PlayerPoseIndex == AIPoseIndex)
	{
		PlayerState->AddTie(CurrentRoundIndex, PlayerPoseIndex);
		AIPlayerState->AddTie(CurrentRoundIndex, AIPoseIndex);
	}
	else if (PlayerPoseIndex == GetWinHandPoseIndex(AIPoseIndex))
	{
		PlayerState->AddWin(CurrentRoundIndex, PlayerPoseIndex);
		AIPlayerState->AddLoss(CurrentRoundIndex, AIPoseIndex);
	}
	else
	{
		PlayerState->AddLoss(CurrentRoundIndex, PlayerPoseIndex);
		AIPlayerState->AddWin(CurrentRoundIndex, AIPoseIndex);
	}
}

void ARPS_GameModeBase::Finished() const
{
	const auto PlayerState = GetPlayerState();
	const auto AIPlayerState = GetAIPlayerState();

	if (!PlayerState || !AIPlayerState)
		return;

	if (PlayerState->GetWins() == AIPlayerState->GetWins())
	{
		PlayerState->SetMatchResult(ERPS_GameMatchResult::Tie);
		AIPlayerState->SetMatchResult(ERPS_GameMatchResult::Tie);
	}
	else if (PlayerState->GetWins() > AIPlayerState->GetWins())
	{
		PlayerState->SetMatchResult(ERPS_GameMatchResult::Win);
		AIPlayerState->SetMatchResult(ERPS_GameMatchResult::Loss);
	}
	else
	{
		PlayerState->SetMatchResult(ERPS_GameMatchResult::Loss);
		AIPlayerState->SetMatchResult(ERPS_GameMatchResult::Win);
	}
}

void ARPS_GameModeBase::Award() const
{
	const auto PlayerState = GetPlayerState();
	const auto AIPlayerState = GetAIPlayerState();

	if (!PlayerState || !AIPlayerState)
		return;

	if (!PlayerPawn || !AIPawn)
		return;

	if (PlayerState->GetMatchResult() != ERPS_GameMatchResult::Win)
	{
		PlayerPawn->SetSimulateHandsPhysics(true);
	}

	if (AIPlayerState->GetMatchResult() != ERPS_GameMatchResult::Win)
	{
		AIPawn->SetSimulateHandsPhysics(true);
	}
}

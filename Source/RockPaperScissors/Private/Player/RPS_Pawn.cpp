// Copyright intvoker. All Rights Reserved.


#include "Player/RPS_Pawn.h"

#include "Hands/RPS_Hand.h"
#include "RPS_GameModeBase.h"

// Sets default values
ARPS_Pawn::ARPS_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	LeftChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("LeftChildActorComponent");
	LeftChildActorComponent->SetupAttachment(GetRootComponent());

	RightChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("RightChildActorComponent");
	RightChildActorComponent->SetupAttachment(GetRootComponent());
}

FTransform ARPS_Pawn::GetLeftHandRelativeTransform() const
{
	return LeftChildActorComponent->GetRelativeTransform();
}

FTransform ARPS_Pawn::GetRightHandRelativeTransform() const
{
	return RightChildActorComponent->GetRelativeTransform();
}

void ARPS_Pawn::SetSimulateHandsPhysics(bool bEnabled) const
{
	LeftHand->ClearHandPose();
	LeftHand->SetSimulateHandPhysics(bEnabled);

	RightHand->ClearHandPose();
	RightHand->SetSimulateHandPhysics(bEnabled);
}

void ARPS_Pawn::ResetHands()
{
	ActiveHand = nullptr;

	SetSimulateHandsPhysics(false);
}

// Called when the game starts or when spawned
void ARPS_Pawn::BeginPlay()
{
	Super::BeginPlay();

	SetupHands();
}

void ARPS_Pawn::SetupHands()
{
	LeftHand = Cast<ARPS_Hand>(LeftChildActorComponent->GetChildActor());
	RightHand = Cast<ARPS_Hand>(RightChildActorComponent->GetChildActor());
}

void ARPS_Pawn::LogLeftHand()
{
	LeftHand->LogHandPose();
}

void ARPS_Pawn::LogRightHand()
{
	RightHand->LogHandPose();
}

void ARPS_Pawn::SetHandPose(int32 PoseIndex)
{
	if (ActiveHand)
	{
		ActiveHand->SetHandPose(PoseIndex);
	}
}

void ARPS_Pawn::ClearHandPose()
{
	if (ActiveHand)
	{
		ActiveHand->ClearHandPose();
	}
}

void ARPS_Pawn::SetSimulateHandPhysics(bool bEnabled)
{
	if (ActiveHand)
	{
		ActiveHand->SetSimulateHandPhysics(bEnabled);
	}
}

void ARPS_Pawn::StartMatch()
{
	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->StartMatch();
	}
}

void ARPS_Pawn::SpawnItemActor()
{
	if (const auto RPS_GameModeBase = GetWorld()->GetAuthGameMode<ARPS_GameModeBase>())
	{
		RPS_GameModeBase->SpawnItemActor();
	}
}

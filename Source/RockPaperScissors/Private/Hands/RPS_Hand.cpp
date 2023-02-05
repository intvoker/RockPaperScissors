// Copyright intvoker. All Rights Reserved.


#include "Hands/RPS_Hand.h"

#include "PoseableHandComponent.h"

// Sets default values
ARPS_Hand::ARPS_Hand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	PoseableHandComponent = CreateDefaultSubobject<UPoseableHandComponent>("PoseableHandComponent");
	PostSetHandType(HandType);
	PoseableHandComponent->SetupAttachment(GetRootComponent());
}

// Called every frame
void ARPS_Hand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPS_Hand::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const auto PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARPS_Hand, HandType))
	{
		PostSetHandType(HandType);
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ARPS_Hand::SetHandPose(FString PoseString) const
{
	PoseableHandComponent->SetPose(PoseString);
}

void ARPS_Hand::ClearHandPose() const
{
	PoseableHandComponent->ClearPose();
}

void ARPS_Hand::PostSetHandType(EOculusHandType HandTypeParam) const
{
	PoseableHandComponent->SkeletonType = HandTypeParam;
	PoseableHandComponent->MeshType = HandTypeParam;
}

// Called when the game starts or when spawned
void ARPS_Hand::BeginPlay()
{
	Super::BeginPlay();

}

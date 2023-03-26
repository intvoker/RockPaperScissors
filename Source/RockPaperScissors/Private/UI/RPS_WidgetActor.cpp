// Copyright intvoker. All Rights Reserved.


#include "UI/RPS_WidgetActor.h"

#include "Components/WidgetComponent.h"

// Sets default values
ARPS_WidgetActor::ARPS_WidgetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ARPS_WidgetActor::SetWidget(UUserWidget* InWidget) const
{
	WidgetComponent->SetWidget(InWidget);
}

// Called when the game starts or when spawned
void ARPS_WidgetActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARPS_WidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

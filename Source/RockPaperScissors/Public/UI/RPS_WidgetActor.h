// Copyright intvoker. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPS_WidgetActor.generated.h"

class UWidgetComponent;

UCLASS()
class ROCKPAPERSCISSORS_API ARPS_WidgetActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPS_WidgetActor();

	void SetWidget(UUserWidget* InWidget) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

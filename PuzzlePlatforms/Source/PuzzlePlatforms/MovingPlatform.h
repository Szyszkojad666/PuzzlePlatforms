// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
	
protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

public:
	AMovingPlatform();

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	bool bTurnedOn = false;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	FVector StartingPoint;

	FVector GlobalTargetLocation;

	bool bComingBack;

	void SwitchDirection();

	void ToggleActive(bool Active);

};

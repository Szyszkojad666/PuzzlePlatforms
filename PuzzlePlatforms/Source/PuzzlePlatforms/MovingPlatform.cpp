// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingPlatform::SwitchDirection()
{
	bComingBack = !bComingBack;
}

void AMovingPlatform::TurnOn()
{
	bTurnedOn = true;
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority() && bTurnedOn)
	{
		FVector Location = GetActorLocation();
		// We need to transform Target Position from Local to Global
		FVector Direction = (GlobalTargetLocation - StartingPoint).GetSafeNormal();
		if (bComingBack)
		{
			Location -= Direction * Speed * DeltaTime;
			SetActorLocation(Location);
			if (Location.PointsAreNear(Location, StartingPoint, 50.0f))
			{
				SwitchDirection();
			}
		}
		else
		{
			Location += Direction * Speed * DeltaTime;
			SetActorLocation(Location);
			if (Location.PointsAreNear(Location, GlobalTargetLocation, 50.0f))
			{
				SwitchDirection();
			}
		}
	}
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		StartingPoint = GetActorLocation();
		GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	}
}
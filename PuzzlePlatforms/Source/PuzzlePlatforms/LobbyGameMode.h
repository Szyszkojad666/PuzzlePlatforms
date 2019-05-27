// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;
	
	uint32 PlayerCount = 0;

private:

	void StartGame();

	float GameStartDelay = 10.0f;
};

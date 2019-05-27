// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "UnrealNames.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++PlayerCount;
	if (PlayerCount >= 3)
	{
		StartGame();
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	--PlayerCount;
}

void ALobbyGameMode::BeginPlay()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartGame, GameStartDelay, false);
}

void ALobbyGameMode::StartGame()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->StartSession(NAME_GameSession);
			UWorld* World = GetWorld();
			if (!ensure(World != NULL)) return;
			bUseSeamlessTravel = true;
			World->ServerTravel("/Game/Maps/L_Game?listen");
		}
	}
}

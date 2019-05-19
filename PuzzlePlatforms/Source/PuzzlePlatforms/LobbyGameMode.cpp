// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	++PlayerCount;
	if (PlayerCount >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("3 players in the lobby"));
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	PlayerCount--;
}

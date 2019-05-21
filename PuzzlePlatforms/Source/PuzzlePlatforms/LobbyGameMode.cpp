// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++PlayerCount;
	if (PlayerCount >= 2)
	{
		UWorld* World = GetWorld();
		if (!ensure(World != NULL)) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/L_Game?listen");
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	--PlayerCount;
}

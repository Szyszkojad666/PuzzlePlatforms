// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionInfo.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"



void USessionInfo::SetServerData(FServerData InServerData)
{
	if (ServerID)
		ServerID->SetText(FText::FromString(InServerData.Name));
	HostName->SetText(FText::FromString(InServerData.HostUserName));
	MaxPlayers->SetText(FText::AsNumber(InServerData.MaxPlayers));
	CurrentPlayers->SetText(FText::AsNumber(InServerData.CurrentPlayers));
}

void USessionInfo::Setup(UMainMenu * MenuOwner)
{
	if (MenuOwner)
		MainMenu = MenuOwner;
}

bool USessionInfo::Initialize()
{
	bool bInitialized = Super::Initialize();
	ServerButton->OnClicked.AddDynamic(this, &USessionInfo::Select);
	return true;
}

void USessionInfo::Select()
{
	MainMenu->SelectIndex(this);
}
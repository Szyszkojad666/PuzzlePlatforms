// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionInfo.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"


void USessionInfo::SetServerID(FText InID)
{
	if (ServerID)
		ServerID->SetText(InID);
	else
		return;
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
// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Runtime/UMG/Public/Components/Button.h"

bool UMainMenu::Initialize()
{
	bool bInitialized = Super::Initialize();
	if (!bInitialized) { return false; }
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::Join);
	return true;
}

void UMainMenu::Host()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::Join()
{
	UE_LOG(LogTemp, Warning, TEXT("Join button clicked"));
}

void UMainMenu::SetMenuInterface(IMenuInterface * MenuInterface)
{
	if (MenuInterface)
	{
		this->MenuInterface = MenuInterface;
	}
}

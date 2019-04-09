// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"

bool UMainMenu::Initialize()
{
	bool bInitialized = Super::Initialize();
	if (!bInitialized) { return false; }
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::Join);
	MenuSwitcherButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchMenu);
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchMenu);
	return true;
}

void UMainMenu::Host()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
		Deactivate();
	}
}

void UMainMenu::Join()
{
	if (MenuInterface != nullptr)
	{
		FString IPAddress = IPAddressField->GetText().ToString();
		MenuInterface->Join(IPAddress);
		Deactivate();
	}
}

void UMainMenu::SwitchMenu()
{
	if (MenuSwitcher->GetActiveWidget() == JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
	else
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
	}
}

void UMainMenu::Setup()
{
	this->AddToViewport();
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}

void UMainMenu::Deactivate()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		HostButton->OnClicked.RemoveAll(this);
		JoinButton->OnClicked.RemoveAll(this);
		MenuSwitcherButton->OnClicked.RemoveAll(this);
		CancelButton->OnClicked.RemoveAll(this);
		RemoveFromParent();
	}
}

void UMainMenu::SetMenuInterface(IMenuInterface * MenuInterface)
{
	if (MenuInterface)
	{
		this->MenuInterface = MenuInterface;
	}
}

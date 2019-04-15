// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool bInitialized = Super::Initialize();
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Cancel);
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::Quit);
	return true;
}

void UInGameMenu::Cancel()
{
	Deactivate();
}

void UInGameMenu::Quit()
{
	if (!ensure(MenuInterface != NULL)) return;
	{
		Deactivate();
		MenuInterface->Quit();
	}
}

void UInGameMenu::Deactivate()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		CancelButton->OnClicked.RemoveAll(this);
		QuitButton->OnClicked.RemoveAll(this);
		RemoveFromParent();
	}
}

void UInGameMenu::SetMenuInterface(IMenuInterface * MenuInterface)
{
	if (MenuInterface)
	{
		this->MenuInterface = MenuInterface;
	}
}

void UInGameMenu::Setup()
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

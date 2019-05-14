// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "MenuSystem/SessionInfo.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/EditableText.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SessionInfoWidgetBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_SessionInfo"));
	if (!ensure(SessionInfoWidgetBPClass.Class != NULL)) return;
	GameSessionInfoWidgetBlueprintClass = SessionInfoWidgetBPClass.Class;
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *SessionInfoWidgetBPClass.Class->GetName());
}

bool UMainMenu::Initialize()
{
	bool bInitialized = Super::Initialize();
	if (!bInitialized) { return false; }
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::Join);
	MenuSwitcherButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchJoinMenu);
	HostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchHostMenu);
	CancelHostButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchHostMenu);
	return true;
}

void UMainMenu::Host()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host(HostNameField->GetText());
		Deactivate();
	}
}

void UMainMenu::SelectIndex(USessionInfo* InWidgetToSelect)
{
	int32 Index = ServerList->GetChildIndex(InWidgetToSelect);
	SelectedIndex = Index;
	UpdateSelection();
	UE_LOG(LogTemp, Warning, TEXT("Selected index is %d"), Index);
}
void UMainMenu::UpdateSelection()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		USessionInfo* ServerRow = Cast<USessionInfo>(ServerList->GetChildAt(i));
		if (ServerRow)
		{
			ServerRow->bSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
void UMainMenu::Join()
{
	if (MenuInterface != nullptr)
	{
		if(SelectedIndex.IsSet())
		MenuInterface->Join(SelectedIndex.GetValue());
	}
}

void UMainMenu::SwitchJoinMenu()
{
	if (MenuSwitcher->GetActiveWidget() == JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
	else
	{
		ServerList->ClearChildren();
		MenuInterface->SearchSessions();
		MenuSwitcher->SetActiveWidget(JoinMenu);
	}
}

void UMainMenu::SwitchHostMenu()
{
	if (MenuSwitcher->GetActiveWidget() == HostMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
	else
	{
		MenuSwitcher->SetActiveWidget(HostMenu);
	}
}

void UMainMenu::Deactivate()
{
	Super::Deactivate();
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		HostButton->OnClicked.RemoveAll(this);
		JoinButton->OnClicked.RemoveAll(this);
		MenuSwitcherButton->OnClicked.RemoveAll(this);
		RemoveFromParent();
	}
}

void UMainMenu::Quit()
{
	MenuInterface->QuitGame();
}

void UMainMenu::Cancel()
{
	SwitchJoinMenu();
}

void UMainMenu::AddServerWidgetToServerList(FServerData ServerData)
{
	UWorld* World = GetWorld();
	if (World)
	{
		USessionInfo* SessionInfo = CreateWidget<USessionInfo>(World, GameSessionInfoWidgetBlueprintClass);
		if (!ensure(SessionInfo != NULL)) return;
		{
			ServerList->AddChild(SessionInfo);
			ServerList->GetChildIndex(SessionInfo);
			SessionInfo->Setup(this);
			SessionInfo->SetServerData(ServerData);
		}
	}
}

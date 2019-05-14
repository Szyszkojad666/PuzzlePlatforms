// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "MenuSystem/MainMenu.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "InGameMenu.h"
#include "OnlineSessionSettings.h"


const static FName SESSION_NAME = TEXT("MyGameSession");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	// Getting the blueprint class is done in the constructor
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_Menu"));
	if (!ensure(MenuWidgetBPClass.Class != NULL)) return;
	MainMenuWidgetBlueprintClass = MenuWidgetBPClass.Class;
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuWidgetBPClass.Class->GetName());

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_InGameMenu"));
	if (!ensure(InGameMenuWidgetBPClass.Class != NULL)) return;
	InGameMenuWidgetBlueprintClass = InGameMenuWidgetBPClass.Class;
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *InGameMenuWidgetBPClass.Class->GetName());
}

void UPuzzlePlatformsGameInstance::Init()
{
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS = %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
}

void UPuzzlePlatformsGameInstance::Host(FText HostName)
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession == nullptr)
		{
			CreateSession(HostName);
		}
		else
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
}

void UPuzzlePlatformsGameInstance::CreateSession(FText HostName)
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = (OnlineSubsystem != nullptr && OnlineSubsystem->GetSubsystemName() == "NULL");
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(TEXT("Test"), HostName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::Join(int32 SessionIndex)
{
	if (SessionInterface.IsValid() && SessionSearch.IsValid())
	{
		MainMenu->Deactivate();
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[SessionIndex]);
	}
}

UMainMenu* UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MainMenuWidgetBlueprintClass!= NULL)) return nullptr;
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuWidgetBlueprintClass);
	if (!ensure(MainMenu != NULL)) return nullptr;
	{
		MainMenu->Setup();
		MainMenu->SetMenuInterface(this);
		return MainMenu;
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuWidgetBlueprintClass != NULL)) return;
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuWidgetBlueprintClass);
	if (!ensure(InGameMenu != NULL)) return;
	{
		InGameMenu->Setup();
		InGameMenu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformsGameInstance::Quit()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel("/Game/ThirdPersonCPP/Blueprints/UI/WBP_Menu", ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::QuitGame()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ConsoleCommand("Exit");
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating game session failed"));
		return;
	}
	else 
	{
		UEngine* Engine = GetEngine();
		if (Engine)
		{
			Engine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Host"));
			UWorld* World = GetWorld();
			if (World)
			{
				World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
			}
		}
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FText NewSessionName = FText::FromName(SessionName);
		CreateSession(NewSessionName);
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> SearchResult = SessionSearch->SearchResults;
			for(auto& Result : SearchResult)
			{
				if (Result.IsSessionInfoValid())
				{
					{
						FServerData ServerData;
						ServerData.Name = Result.GetSessionIdStr();
						ServerData.HostUserName = Result.Session.OwningUserName;
						ServerData.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
						ServerData.CurrentPlayers = ServerData.MaxPlayers - Result.Session.NumOpenPublicConnections;
						FString Data;
						if (Result.Session.SessionSettings.Get(TEXT("Test"), Data))
						{
							UE_LOG(LogTemp, Warning, TEXT("Data found in settings: %s"), *Data);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Didn't get expected data"));
						}
						MainMenu->AddServerWidgetToServerList(ServerData);
					}
				}
			}
	}
	UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete"));
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC && Result == EOnJoinSessionCompleteResult::Success)
	{
		FString Address;
		if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Address))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
			return;
		}
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::SearchSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100; // because we are using a shared app id we need more results to find our game
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


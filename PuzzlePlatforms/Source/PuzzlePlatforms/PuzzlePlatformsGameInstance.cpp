// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "MenuSystem/MainMenu.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "InGameMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

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
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OSS = %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
		}
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession == nullptr)
		{
			CreateSession();
		}
		else
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Joining %s"), *Address)); // printf is needed to convert TEXT into FString and print it together with Address
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
		CreateSession();
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
						MainMenu->AddServerWidgetToServerList(FText::FromString(Result.GetSessionIdStr()));
					}
				}
			}
	}
	UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete"));
}

void UPuzzlePlatformsGameInstance::SearchSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "MenuSystem/MainMenu.h"
#include "OnlineSubsystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "InGameMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"


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
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->CreateSession(0, TEXT("MyGameSession"), SessionSettings);
		}
	}
}

void UPuzzlePlatformsGameInstance::Host()
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

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Joining %s"), *Address)); // printf is needed to convert TEXT into FString and print it together with Address
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MainMenuWidgetBlueprintClass!= NULL)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MainMenuWidgetBlueprintClass);
	if (!ensure(Menu != NULL)) return;
	{
		Menu->Setup();
		Menu->SetMenuInterface(this);
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuWidgetBlueprintClass != NULL)) return;
	UInGameMenu* Menu = CreateWidget<UInGameMenu>(this, InGameMenuWidgetBlueprintClass);
	if (!ensure(Menu != NULL)) return;
	{
		Menu->Setup();
		Menu->SetMenuInterface(this);
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
	if (bWasSuccessful)
	{
		Host();
	}
}


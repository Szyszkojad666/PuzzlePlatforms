// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "MenuSystem/MainMenu.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	// Getting the blueprint class is done in the constructor
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_Menu"));
	if (!ensure(MenuWidgetBPClass.Class != NULL)) return;
	MainMenuWidgetBlueprintClass = MenuWidgetBPClass.Class;
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuWidgetBPClass.Class->GetName());
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));
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

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MainMenuWidgetBlueprintClass!= NULL)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MainMenuWidgetBlueprintClass);
	if (!ensure(Menu != NULL)) return;
	{
		Menu->Setup();
		Menu->SetMenuInterface(this);
	}
}


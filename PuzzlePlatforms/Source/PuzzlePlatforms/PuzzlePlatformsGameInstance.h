// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MainMenu.h"
#include "OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	
	UPuzzlePlatformsGameInstance();

	virtual void Init();

	UFUNCTION(exec) //exec allows this function to show in console
	void Host() override;

	void CreateSession();

	UFUNCTION(exec)
	void Join(int32 SessionIndex) override;

	UFUNCTION(BlueprintCallable) //exec allows this function to show in console
	UMainMenu* LoadMenuWidget();

	UFUNCTION(BlueprintCallable) //exec allows this function to show in console
	void LoadInGameMenu();

	UFUNCTION()
	void Quit() override;

	UFUNCTION()
	void QuitGame() override;

	UFUNCTION()
	void SearchSessions() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void OnFindSessionsComplete(bool bWasSuccessful);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	

private:
	
	TSubclassOf<UUserWidget> MainMenuWidgetBlueprintClass;

	TSubclassOf<UUserWidget> InGameMenuWidgetBlueprintClass;
	
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	UInGameMenu* InGameMenu = nullptr;

	UMainMenu* MainMenu = nullptr;
	
};

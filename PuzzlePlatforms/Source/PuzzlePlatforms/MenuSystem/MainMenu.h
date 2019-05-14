// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameMenu.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

class USessionInfo;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UInGameMenu
{
	GENERATED_BODY()

public:
	
	void AddServerWidgetToServerList(FServerData ServerData);
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SelectIndex(USessionInfo* InWidgetToSelect);

	void UpdateSelection();

	virtual void Deactivate() override;

private:
	// This meta binds the blueprint property to c++, as long as it is named the same in Blueprint
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MenuSwitcherButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* IPAddressField;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* HostNameField;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	TSubclassOf<class UUserWidget> GameSessionInfoWidgetBlueprintClass;

	TOptional<int32> SelectedIndex;

protected:
	
	UFUNCTION()
	void Host();

	UFUNCTION()
	void Join();

	UFUNCTION()
	void SwitchJoinMenu();

	UFUNCTION()
	void SwitchHostMenu();

	virtual void Quit() override;

	virtual void Cancel() override;

	virtual bool Initialize() override;

	
};

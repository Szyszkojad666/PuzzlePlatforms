// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameMenu.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
class USessionInfo;
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UInGameMenu
{
	GENERATED_BODY()

public:
	
	void AddServerWidgetToServerList(FText Address);
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SelectIndex(USessionInfo* InWidgetToSelect);

private:
	// This meta binds the blueprint property to c++, as long as it is named the same in Blueprint
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MenuSwitcherButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* IPAddressField;

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
	void SwitchMenu();

	virtual void Deactivate() override;

	virtual void Quit() override;

	virtual void Cancel() override;

	virtual bool Initialize() override;

	
};

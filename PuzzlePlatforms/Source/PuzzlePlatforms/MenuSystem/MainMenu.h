// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameMenu.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UInGameMenu
{
	GENERATED_BODY()

protected:
virtual bool Initialize() override;

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
};

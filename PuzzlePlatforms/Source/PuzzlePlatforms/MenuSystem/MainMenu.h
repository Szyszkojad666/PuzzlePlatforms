// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
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
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	
	IMenuInterface* MenuInterface;

public:
	UFUNCTION()
	void Host();

	UFUNCTION()
	void Join();

	UFUNCTION()
	void SwitchMenu();

	UFUNCTION()
	void Setup();

	UFUNCTION()
	void Deactivate();
	
	void SetMenuInterface(IMenuInterface* MenuInterface);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuInterface.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	
	IMenuInterface* MenuInterface;

	UFUNCTION()
	virtual void Cancel();

	UFUNCTION()
	virtual void Quit();

	virtual void Deactivate();

private:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	
	void Setup();
};

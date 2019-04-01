// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MainMenu.h"
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
	void Host();

	UFUNCTION(exec)
	void Join(const FString& Address);

	UFUNCTION(BlueprintCallable) //exec allows this function to show in console
	void LoadMenu();

private:
	TSubclassOf<UUserWidget> MainMenuWidgetBlueprintClass;
	
	
};

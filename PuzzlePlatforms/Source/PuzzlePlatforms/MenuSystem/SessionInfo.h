// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionInfo.generated.h"

/**
 * 
 */
class UMainMenu;

UCLASS()
class PUZZLEPLATFORMS_API USessionInfo : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerID;
	
public:
	void SetServerID(FText InID);

	UPROPERTY(meta = (BindWidget))
	class UButton* ServerButton;

	void Setup(UMainMenu* MenuOwner);

private:
	UMainMenu* MainMenu = nullptr;
	
	virtual bool Initialize() override;
	
	UFUNCTION()
	void Select();
};

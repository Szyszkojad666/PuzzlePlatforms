// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionInfo.generated.h"

/**
 * 
 */
class UMainMenu;
struct FServerData;

UCLASS()
class PUZZLEPLATFORMS_API USessionInfo : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerID;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxPlayers;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPlayers;
	
public:
	void SetServerData(FServerData InServerData);

	UPROPERTY(meta = (BindWidget))
	class UButton* ServerButton;

	UPROPERTY(BlueprintReadOnly)
	bool bSelected;

	void Setup(UMainMenu* MenuOwner);

private:
	UMainMenu* MainMenu = nullptr;
	
	virtual bool Initialize() override;
	
	UFUNCTION()
	void Select();
};

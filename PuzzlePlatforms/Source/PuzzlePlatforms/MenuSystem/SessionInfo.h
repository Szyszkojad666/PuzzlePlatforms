// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionInfo.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API USessionInfo : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerID;
	
public:
	void SetServerID(FText InID);
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionInfo.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"


void USessionInfo::SetServerID(FText InID)
{
	if (ServerID)
		ServerID->SetText(InID);
	else
		return;
}

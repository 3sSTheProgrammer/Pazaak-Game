// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundEndWidget.h"

#include "Components/TextBlock.h"

void URoundEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URoundEndWidget::RemoveFromParent, 3);
}

void URoundEndWidget::SetRoundResult(FString RoundWinner)
{
	//Identify self name
	FString PlayerName;
	if (GetWorld()->IsServer())
	{
		PlayerName = "Player1";
	}
	else
	{
		PlayerName = "Player2";
	}

	//Select result text
	FString ResultMessage;
	if (RoundWinner == "Draw")
	{
		ResultMessage = "Draw";
	}
	else if (RoundWinner == PlayerName)
	{
		ResultMessage = "You won round";
	}
	else
	{
		ResultMessage = "You lost round"; 
	}

	//Show message
	TBRoundResult->SetText(FText::FromString(ResultMessage));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundEndWidget.h"
#include "PointManager.h"
#include "Components/TextBlock.h"

void URoundEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URoundEndWidget::RemoveFromParent, 3);
}

void URoundEndWidget::SetRoundResult(int32 RoundWinner)
{
	//Identify self name
	int32 PlayerName;
	if (GetWorld()->IsServer())
	{
		PlayerName = EPlayerEnum::Player1;
	}
	else
	{
		PlayerName = EPlayerEnum::Player2;
	}

	//Select result text
	FString ResultMessage;
	if (RoundWinner == EPlayerEnum::PlayerNone)
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

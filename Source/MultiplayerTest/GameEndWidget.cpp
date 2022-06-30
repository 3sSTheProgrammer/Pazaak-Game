// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetPause(true);
	//TODO bind buttons
}

void UGameEndWidget::SetMatchResult(FString MatchWinner)
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
	if (MatchWinner == PlayerName)
	{
		ResultMessage = "YOU WON";
	}
	else
	{
		ResultMessage = "YOU LOST"; 
	}

	//Show message
	TBMatchResult->SetText(FText::FromString(ResultMessage));
}

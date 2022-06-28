// Fill out your copyright notice in the Description page of Project Settings.

#include "TestUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TestCharacter.h"
#include "Components/Image.h"
#include "PointManager.h"

void UTestUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Binding button delegate
	if (ButtonEndTurn)
	{
		ButtonEndTurn->OnClicked.AddDynamic(this, &UTestUserWidget::Server_ButtonEndTurnOnClick);
	}

	// Filling arrays
	CardValues.Add(CardEmpty);
	CardValues.Add(CardOne);
	CardValues.Add(CardTwo);
	CardValues.Add(CardThree);
	CardValues.Add(CardFour);
	CardValues.Add(CardFive);
	CardValues.Add(CardSix);
	CardValues.Add(CardSeven);
	CardValues.Add(CardEight);
	CardValues.Add(CardNine);
	
	Player1CardSlotsArray.Add(ImgPlayer1Slot1);
	Player1CardSlotsArray.Add(ImgPlayer1Slot2);
	Player1CardSlotsArray.Add(ImgPlayer1Slot3);
	Player1CardSlotsArray.Add(ImgPlayer1Slot4);
	Player1CardSlotsArray.Add(ImgPlayer1Slot5);
	Player1CardSlotsArray.Add(ImgPlayer1Slot6);
	Player1CardSlotsArray.Add(ImgPlayer1Slot7);
	Player1CardSlotsArray.Add(ImgPlayer1Slot8);
	Player1CardSlotsArray.Add(ImgPlayer1Slot9);
	
	Player2CardSlotsArray.Add(ImgPlayer2Slot1);
	Player2CardSlotsArray.Add(ImgPlayer2Slot2);
	Player2CardSlotsArray.Add(ImgPlayer2Slot3);
	Player2CardSlotsArray.Add(ImgPlayer2Slot4);
	Player2CardSlotsArray.Add(ImgPlayer2Slot5);
	Player2CardSlotsArray.Add(ImgPlayer2Slot6);
	Player2CardSlotsArray.Add(ImgPlayer2Slot7);
	Player2CardSlotsArray.Add(ImgPlayer2Slot8);
	Player2CardSlotsArray.Add(ImgPlayer2Slot9);
	
	InitPointManager();

	// Identify player name
	if (GetWorld()->IsServer())
	{
		PlayerName = "Player1";
	}
	else
	{
		PlayerName = "Player2";
	}
}

void UTestUserWidget::UpdatePlayerInterface(TArray<int32> Player1CardSlots, TArray<int32> Player2CardSlots)
{
	// Fill card slots
	for (int i = 0; i < Player1CardSlots.Num(); ++i)
	{
		FillSlot(Player1CardSlotsArray[i], Player1CardSlots[i]);
	}
	for (int i = 0; i < Player2CardSlots.Num(); ++i)
	{
		FillSlot(Player2CardSlotsArray[i], Player2CardSlots[i]);
	}

	//TODO: Show total scores

	//TODO: Enable/disable buttons
	
}



void UTestUserWidget::FillSlot(UImage* CardSlot, int32 CardValue)
{
	if (CardSlot && CardValue)
	{
		CardSlot->SetBrushFromTexture(CardValues[CardValue]);
	}
}

bool UTestUserWidget::Server_ButtonEndTurnOnClick_Validate()
{
	return true;
}


void UTestUserWidget::Server_ButtonEndTurnOnClick_Implementation()
{
	if (!PointManager)
	{
		InitPointManager();
	}
	PointManager->EndTurn();
}

void UTestUserWidget::InitPointManager()
{
	TArray<AActor*> PointManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointManager::StaticClass(),PointManagers);
	if (PointManagers.Num() > 0)
	{
		PointManager = Cast<APointManager>(PointManagers[0]);
	}
}

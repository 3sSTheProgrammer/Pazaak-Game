// Fill out your copyright notice in the Description page of Project Settings.

#include "TestUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TestActor.h"
#include "TestCharacter.h"
#include "Components/Image.h"
#include "GameFramework/GameSession.h"

void UTestUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (TestButton)
	{
		TestButton->OnClicked.AddDynamic(this, &UTestUserWidget::TestButtonOnClick);
	}

	
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
	//ImgPlayer1Slot1->SetBrushFromTexture(CardOne);
}

void UTestUserWidget::UpdateCounter(int Count)
{
	TestTextBlock->SetText(FText::AsNumber(Count));
}

void UTestUserWidget::UpdatePointsCounters(int32 SelfPoints, int32 OpponentPoints)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePointsCounters %d %d"), SelfPoints, OpponentPoints);
	TextSelfPoints->SetText(FText::AsNumber(SelfPoints));
	TextOpponentPoints->SetText(FText::AsNumber(OpponentPoints));
}

void UTestUserWidget::UpdateCardSlots(TArray<int32> Player1CardSlots, TArray<int32> Player2CardSlots)
{
	for (int i = 0; i < Player1CardSlots.Num(); ++i)
	{
		FillSlot(Player1CardSlotsArray[i], Player1CardSlots[i]);
		
		//Player1CardSlotsArray[i]->SetText(FText::AsNumber(Player1CardSlots[i]));
	}
	for (int i = 0; i < Player2CardSlots.Num(); ++i)
	{
		FillSlot(Player2CardSlotsArray[i], Player2CardSlots[i]);
		//Player2CardSlotsArray[i]->SetText(FText::AsNumber(Player2CardSlots[i]));
	}
}

void UTestUserWidget::TestButtonOnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("TestButton was pressed by %s"), *GetOwningPlayer()->GetName());
	
	TArray<AActor*> TestCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestCharacter::StaticClass(),TestCharacters);
	if (TestCharacters.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d test characters"), TestCharacters.Num());
		//GetPlayerControllerFromNetId()
		// for (AActor* Actor : TestActors)
		// {
		// 	if (ATestActor* CastedActor = Cast<ATestActor>(Actor))
		// 	{
		// 		if (GetWorld()->IsServer())
		// 		{
		// 			CastedActor->UseParamChange();
		// 		}
		// 		else
		// 		{
		// 			UE_LOG(LogTemp, Warning, TEXT("calling Server_TestButtonOnClick"));
		// 			Server_TestButtonOnClick(CastedActor);
		// 		}
		// 	}
		// }
	}
	
}

void UTestUserWidget::FillSlot(UImage* CardSlot, int32 CardValue)
{
	if (CardSlot)
	{
		CardSlot->SetBrushFromTexture(CardValues[CardValue]);
	}
}

bool UTestUserWidget::Server_TestButtonOnClick_Validate(ATestActor* TestActor)
{
	return true;
}

void UTestUserWidget::Server_TestButtonOnClick_Implementation(ATestActor* TestActor)
{
	if (TestActor)
	{
		TestActor->UseParamChange();
	}
}

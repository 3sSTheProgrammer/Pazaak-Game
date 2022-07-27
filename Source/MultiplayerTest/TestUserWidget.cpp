// Fill out your copyright notice in the Description page of Project Settings.

#include "TestUserWidget.h"
#include "PazaakPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

void UTestUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Binding button delegate
	if (ButtonEndTurn)
	{
		ButtonEndTurn->OnClicked.AddDynamic(this, &UTestUserWidget::ButtonEndTurnOnClick);
	}
	if (ButtonPass)
	{
		ButtonPass->OnClicked.AddDynamic(this, &UTestUserWidget::ButtonPassOnClick);
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

	RoundScoresTextures.Add(TextureRoundScore0);
	RoundScoresTextures.Add(TextureRoundScore1);
	RoundScoresTextures.Add(TextureRoundScore2);
	RoundScoresTextures.Add(TextureRoundScore3);

	Player1Interface.RoundScore = ImgPlayer1RoundsScore;
	Player1Interface.TableScore = TBPlayer1TableScore;
	Player1Interface.CardSlots.Add(ImgPlayer1Slot1);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot2);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot3);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot4);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot5);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot6);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot7);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot8);
	Player1Interface.CardSlots.Add(ImgPlayer1Slot9);
	
	Player2Interface.RoundScore = ImgPlayer2RoundsScore;
	Player2Interface.TableScore = TBPlayer2TableScore;
	Player2Interface.CardSlots.Add(ImgPlayer2Slot1);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot2);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot3);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot4);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot5);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot6);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot7);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot8);
	Player2Interface.CardSlots.Add(ImgPlayer2Slot9);
	
	PlayerInterfaceMap.Add(EPlayerEnum::Player1, &Player1Interface);
	PlayerInterfaceMap.Add(EPlayerEnum::Player2, &Player2Interface);
	
	// Player1CardSlotsArray.Add(ImgPlayer1Slot1);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot2);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot3);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot4);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot5);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot6);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot7);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot8);
	// Player1CardSlotsArray.Add(ImgPlayer1Slot9);
	//
	// Player2CardSlotsArray.Add(ImgPlayer2Slot1);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot2);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot3);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot4);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot5);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot6);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot7);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot8);
	// Player2CardSlotsArray.Add(ImgPlayer2Slot9);
	
	InitPlayerController();
	
	// Identify player name
	if (GetWorld()->IsServer())
	{
		PlayerName = EPlayerEnum::Player1;
	}
	else
	{
		PlayerName = EPlayerEnum::Player2;
	}
}

void UTestUserWidget::UpdatePlayerInterfaceOld(TArray<int32> Player1CardSlots,
	TArray<int32> Player2CardSlots, FString ActivePlayer, int32 Player1TableScore,
	int32 Player2TableScore, int32 Player1RoundsScore, int32 Player2RoundsScore)
{
	// Fill card slots
	//TODO try to optimize
	//UE_LOG(LogTemp, Warning, TEXT("Updating interface whatever"));
	// if (Player1CardSlots.Num() == 0 && Player2CardSlots.Num() == 0)
	// {
	// 	for (int i = 0; i < Player1CardSlotsArray.Num(); ++i)
	// 	{
	// 		FillSlot(Player1CardSlotsArray[i], 0);
	// 	}
	// 	for (int i = 0; i < Player2CardSlotsArray.Num(); ++i)
	// 	{
	// 		FillSlot(Player2CardSlotsArray[i], 0);
	// 	}
	// }
	// else
	// {
	// 	for (int i = 0; i < Player1CardSlots.Num(); ++i)
	// 	{
	// 		FillSlot(Player1CardSlotsArray[i], Player1CardSlots[i]);
	// 	}
	// 	for (int i = 0; i < Player2CardSlots.Num(); ++i)
	// 	{
	// 		FillSlot(Player2CardSlotsArray[i], Player2CardSlots[i]);
	// 	}
	// }
	
	//Show total scores
	// if (TBPlayer1TableScore)
	// {
	// 	TBPlayer1TableScore->SetText(FText::AsNumber(Player1TableScore));
	// }
	// if (TBPlayer2TableScore)
	// {
	// 	TBPlayer2TableScore->SetText(FText::AsNumber(Player2TableScore));
	// }
	
	//Enable/disable buttons
	//UE_LOG(LogTemp, Warning, TEXT("I am %s; Active player is %s"), *PlayerName, *ActivePlayer);
	// if (ActivePlayer == PlayerName && !ButtonEndTurn->GetIsEnabled())
	// {
	// 	//UE_LOG(LogTemp, Warning, TEXT("Enabling buttons"));
	// 	ButtonEndTurn->SetIsEnabled(true);
	// 	ButtonPass->SetIsEnabled(true);
	// }
	// else if (ActivePlayer != PlayerName && ButtonEndTurn->GetIsEnabled()) 
	// {
	// 	//UE_LOG(LogTemp, Warning, TEXT("Disabling buttons"));
	// 	ButtonEndTurn->SetIsEnabled(false);
	// 	ButtonPass->SetIsEnabled(false);
	// }

	//Show round scores
	// if (Player1RoundsScore <= 3 && Player2RoundsScore <= 3)
	// {
	// 	if (ImgPlayer1RoundsScore)
	// 	{
	// 		ImgPlayer1RoundsScore->SetBrushFromTexture(RoundScoresTextures[Player1RoundsScore]);
	// 	}
	// 	if (ImgPlayer2RoundsScore)
	// 	{
	// 		ImgPlayer2RoundsScore->SetBrushFromTexture(RoundScoresTextures[Player2RoundsScore]);
	// 	}
	// }
	
}

void UTestUserWidget::UpdatePlayerInterface(FPMPlayerState Player)
{
	// Update table score
	PlayerInterfaceMap[Player.ID]->TableScore->SetText(FText::AsNumber(Player.TableScore));

	// Update card slots
	// TODO try to optimize
	if (Player.CardSlots.Num() == 0)
	{
		for (int i = 0; i < PlayerInterfaceMap[Player.ID]->CardSlots.Num(); ++i)
		{
			FillSlot(PlayerInterfaceMap[Player.ID]->CardSlots[i], 0);
		}
	}
	else
	{
		for (int i = 0; i < Player.CardSlots.Num(); ++i)
		{
			FillSlot(PlayerInterfaceMap[Player.ID]->CardSlots[i], Player.CardSlots[i]);
		}
	}

	// Update round score
	// TODO optimize?
	if (Player.RoundsScore <= 3)
	{
		if (PlayerInterfaceMap[Player.ID]->RoundScore)
		{
			PlayerInterfaceMap[Player.ID]->RoundScore->SetBrushFromTexture(RoundScoresTextures[Player.RoundsScore]);
		}
	}
}

void UTestUserWidget::UpdateTableScores(int32 Player1TableScore, int32 Player2TableScore)
{
	if (TBPlayer1TableScore)
	{
		TBPlayer1TableScore->SetText(FText::AsNumber(Player1TableScore));
	}
	if (TBPlayer2TableScore)
	{
		TBPlayer2TableScore->SetText(FText::AsNumber(Player2TableScore));
	}
}

void UTestUserWidget::UpdateButtons(int32 ActivePlayer)
{
	//UE_LOG(LogTemp, Warning, TEXT("I am %d, Active player is %d"), !GetWorld()->IsServer(), ActivePlayer);
	if (ActivePlayer == PlayerName && !(ButtonEndTurn->GetIsEnabled()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enabling buttons"));
		ButtonEndTurn->SetIsEnabled(true);
		ButtonPass->SetIsEnabled(true);
	}
	else if (ActivePlayer != PlayerName && ButtonEndTurn->GetIsEnabled())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Disabling buttons"));
		ButtonEndTurn->SetIsEnabled(false);
		ButtonPass->SetIsEnabled(false);
	}
	
}

void UTestUserWidget::UpdateRoundScores(int32 Player1RoundsScore, int32 Player2RoundsScore)
{
	if (Player1RoundsScore <= 3 && Player2RoundsScore <= 3)
	{
		if (ImgPlayer1RoundsScore)
		{
			ImgPlayer1RoundsScore->SetBrushFromTexture(RoundScoresTextures[Player1RoundsScore]);
		}
		if (ImgPlayer2RoundsScore)
		{
			ImgPlayer2RoundsScore->SetBrushFromTexture(RoundScoresTextures[Player2RoundsScore]);
		}
	}
}

// void UTestUserWidget::UpdatePlayersCardSlots(TArray<int32> Player1CardSlots, TArray<int32> Player2CardSlots)
// {
// 	if (Player1CardSlots.Num() == 0 && Player2CardSlots.Num() == 0)
// 	{
// 		for (int i = 0; i < Player1CardSlotsArray.Num(); ++i)
// 		{
// 			FillSlot(Player1CardSlotsArray[i], 0);
// 		}
// 		for (int i = 0; i < Player2CardSlotsArray.Num(); ++i)
// 		{
// 			FillSlot(Player2CardSlotsArray[i], 0);
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0; i < Player1CardSlots.Num(); ++i)
// 		{
// 			FillSlot(Player1CardSlotsArray[i], Player1CardSlots[i]);
// 		}
// 		for (int i = 0; i < Player2CardSlots.Num(); ++i)
// 		{
// 			FillSlot(Player2CardSlotsArray[i], Player2CardSlots[i]);
// 		}
// 	}
// }

void UTestUserWidget::FillSlot(UImage* CardSlot, int32 CardValue)
{
	if (CardSlot)
	{
		CardSlot->SetBrushFromTexture(CardValues[CardValue]);
	}
}

void UTestUserWidget::ButtonEndTurnOnClick()
{
	if(!PazaakPlayerController)
	{
		InitPlayerController();
	}
	if (GetWorld()->IsServer())
	{
		PazaakPlayerController->EndTurn();
	}
	else
	{
		PazaakPlayerController->Server_EndTurn();
	}
	
}

void UTestUserWidget::ButtonPassOnClick()
{
	if (!PazaakPlayerController)
	{
		InitPlayerController();
	}
	if (GetWorld()->IsServer())
	{
		PazaakPlayerController->Pass();
	}
	else
	{
		PazaakPlayerController->Server_Pass();
	}
}

void UTestUserWidget::InitPlayerController()
{
	if (APlayerController* SomePlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (APazaakPlayerController* PlayerController = Cast<APazaakPlayerController>(SomePlayerController))
		{
			PazaakPlayerController = PlayerController;
		}	
	}
}


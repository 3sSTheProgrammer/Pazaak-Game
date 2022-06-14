// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestUserWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;
class ATestActor;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UTestUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//FOR TESTS
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TestTextBlock;
	UPROPERTY(meta = (BindWidget))
	UButton* TestButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextSelfPoints;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextOpponentPoints;

	//FOR TESTS
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1CardSlot0;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1CardSlot1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1CardSlot2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1CardSlot3;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2CardSlot0;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2CardSlot1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2CardSlot2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2CardSlot3;

	//Players card slots
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot1;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot2;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot3;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot4;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot5;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot6;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot7;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot8;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1Slot9;

	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot1;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot2;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot3;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot4;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot5;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot6;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot7;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot8;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2Slot9;

	//Card textures
	UPROPERTY(EditAnywhere)
	UTexture2D* CardEmpty;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardOne;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardTwo;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardThree;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardFour;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardFive;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardSix;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardSeven;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardEight;
	UPROPERTY(EditAnywhere)
	UTexture2D* CardNine;

	//Arrays
	TArray<UTexture2D*> CardValues;
	TArray<UImage*> Player1CardSlotsArray;
	TArray<UImage*> Player2CardSlotsArray;
public:
	virtual void NativeConstruct() override;

	void UpdateCounter(int Count);

	UFUNCTION()
	void UpdatePointsCounters(int32 SelfPoints, int32 OpponentPoints);

	UFUNCTION()
	void UpdateCardSlots(TArray<int32> Player1CardSlots, TArray<int32> Player2CardSlots);
	
protected:
	
	UFUNCTION()
	void TestButtonOnClick();

	void FillSlot(UImage* CardSlot, int32 CardValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TestButtonOnClick(ATestActor* TestActor);
	bool Server_TestButtonOnClick_Validate(ATestActor* TestActor);
	void Server_TestButtonOnClick_Implementation(ATestActor* TestActor);
	
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointManager.h"
#include "Blueprint/UserWidget.h"
#include "TestUserWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class APazaakPlayerController;

USTRUCT()
struct FPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY()
	UTextBlock* TableScore;
	UPROPERTY()
	TArray<UImage*> CardSlots;
	UPROPERTY()
	UImage* RoundScore;
};
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UTestUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//APointManager* PointManager;

	//Saved for efficiency
	UPROPERTY()
	APazaakPlayerController* PazaakPlayerController;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonEndTurn;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonPass;

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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TBPlayer1TableScore;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TBPlayer2TableScore;

	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer1RoundsScore;
	UPROPERTY(meta = (BindWidget))
	UImage* ImgPlayer2RoundsScore;
	
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

	//Round scores textures
	UPROPERTY(EditAnywhere)
	UTexture2D* TextureRoundScore0;
	UPROPERTY(EditAnywhere)
	UTexture2D* TextureRoundScore1;
	UPROPERTY(EditAnywhere)
	UTexture2D* TextureRoundScore2;
	UPROPERTY(EditAnywhere)
	UTexture2D* TextureRoundScore3;

	// Player interface structs
	FPlayerInterface Player1Interface;
	FPlayerInterface Player2Interface;
	TMap<int32, FPlayerInterface*> PlayerInterfaceMap;
	
	//Arrays of textures
	UPROPERTY()
	TArray<UTexture2D*> CardValues;
	UPROPERTY()
	TArray<UTexture2D*> RoundScoresTextures;
	//TArray<UImage*> Player1CardSlotsArray;
	//TArray<UImage*> Player2CardSlotsArray;

	int32 PlayerName;
public:
	virtual void NativeConstruct() override;

	//Called in multicast function to update player interface according to game state
	UFUNCTION()
	void UpdatePlayerInterfaceOld(TArray<int32> Player1CardSlots, TArray<int32> Player2CardSlots, FString ActivePlayer,
		int32 Player1TableScore, int32 Player2TableScore, int32 Player1RoundsScore, int32 Player2RoundsScore);

	UFUNCTION()
	void UpdatePlayerInterface(FPMPlayerState Player);
	
	UFUNCTION()
	void UpdateTableScores(int32 Player1TableScore, int32 Player2TableScore);

	UFUNCTION()
	void UpdateButtons(int32 ActivePlayer);

	UFUNCTION()
	void UpdateRoundScores(int32 Player1RoundsScore, int32 Player2RoundsScore);

// 	UFUNCTION()
// 	void UpdatePlayersCardSlots(TArray<int32> Player1CardSlots, TArray<int32> Player2CardSlots);
	protected:

	//Used in UpdatePlayerInterface
	void FillSlot(UImage* CardSlot, int32 CardValue);

	//Called when ButtonEndTurn is pressed
	UFUNCTION()
	void ButtonEndTurnOnClick();

	//Called when ButtonPass is pressed
	UFUNCTION()
	void ButtonPassOnClick();
	
	// UFUNCTION(Server, Reliable, WithValidation)
	// void Server_ButtonEndTurnOnClick();
	// bool Server_ButtonEndTurnOnClick_Validate();
	// void Server_ButtonEndTurnOnClick_Implementation();

	//Finds pazaak player controller
	void InitPlayerController();
	
	
};



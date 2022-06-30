// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointManager.generated.h"

class UTestUserWidget;

UCLASS()
class MULTIPLAYERTEST_API APointManager : public AActor
{
	GENERATED_BODY()

protected:

	// Current player cards
	UPROPERTY(Replicated)
	TArray<int32> Player1CardSlots = {};
	UPROPERTY(Replicated)
	TArray<int32> Player2CardSlots = {};
	UPROPERTY(Replicated)
	int32 Player1TableScore{ 0 };
	UPROPERTY(Replicated)
	int32 Player2TableScore{ 0 };
	UPROPERTY(Replicated)
	int32 Player1RoundsScore{ 0 };
	UPROPERTY(Replicated)
	int32 Player2RoundsScore{ 0 };

	UPROPERTY(Replicated)
	FString ActivePlayer;
	UPROPERTY()
	FString PlayerBeganPreviousRound;
	
	//Player passed statuses
	UPROPERTY(Replicated)
	bool Player1Passed{ false };
	UPROPERTY(Replicated)
	bool Player2Passed{ false };

	//Widgets
	UPROPERTY()
	UTestUserWidget* GameInterface;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> RoundEndWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> MatchEndWidget;
	
public:	
	// Sets default values for this actor's properties
	APointManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Finds game interface
	void InitGameInterface();

	//
	void GetCardFromDeck(FString Player);

	// Multicast update of interface after changes in game state
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_UpdateInterface();
	bool Multi_UpdateInterface_Validate();
	void Multi_UpdateInterface_Implementation();

	// Multicast creating of round result widget 
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ShowRoundResult(const FString& RoundWinner);
	bool Multi_ShowRoundResult_Validate(const FString& RoundWinner);
	void Multi_ShowRoundResult_Implementation(const FString& RoundWinner);

	// Multicast creating of match result widget 
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ShowMatchResult(const FString& RoundWinner);
	bool Multi_ShowMatchResult_Validate(const FString& RoundWinner);
	void Multi_ShowMatchResult_Implementation(const FString& RoundWinner);
public:	

	// Defines replicated params
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Used to end turn for players
	void EndTurn();

	//Used when player passes
	void Pass();

	//Makes round end
	void EndRound();

	//Resets game state to start a new round
	void ResetGame();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointManager.generated.h"

class UTestUserWidget;

USTRUCT()
struct FPMPlayerState
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ID{ -1 };
	UPROPERTY()
	TArray<int32> CardSlots = {};
	UPROPERTY()
	int32 TableScore{ 0 };
	UPROPERTY()
	int32 RoundsScore{ 0 };
	UPROPERTY()
	bool Passed{ false };
};

UENUM()
enum EPlayerEnum
{
	PlayerNone = -1,
	Player1 = 0,
	Player2 = 1,
};


UCLASS()
class MULTIPLAYERTEST_API APointManager : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Player1Updated)
	FPMPlayerState Player1;
	UPROPERTY(ReplicatedUsing = OnRep_Player2Updated)
	FPMPlayerState Player2;
	UPROPERTY(ReplicatedUsing = OnRep_ActivePlayerUpdated)
	int32 ActivePlayerNum;
	
	TMap<int32, FPMPlayerState*> PlayerMap;
	
	// Current player cards
	// UPROPERTY(ReplicatedUsing = OnRep_PlayersCardSlotsUpdated)
	// TArray<int32> Player1CardSlots = {};
	// UPROPERTY(ReplicatedUsing = OnRep_PlayersCardSlotsUpdated)
	// TArray<int32> Player2CardSlots = {};
	// UPROPERTY(ReplicatedUsing = OnRep_PlayersTableScoreUpdated)
	// int32 Player1TableScore{ 0 };
	// UPROPERTY(ReplicatedUsing = OnRep_PlayersTableScoreUpdated)
	// int32 Player2TableScore{ 0 };
	// UPROPERTY(ReplicatedUsing = OnRep_PlayersRoundsScoresUpdated)
	// int32 Player1RoundsScore{ 0 };
	// UPROPERTY(ReplicatedUsing = OnRep_PlayersRoundsScoresUpdated)
	// int32 Player2RoundsScore{ 0 };
	//
	// UPROPERTY(ReplicatedUsing = OnRep_ActivePlayerUpdated)
	// int32 ActivePlayer;
	UPROPERTY()
	int32 PlayerBeganPreviousRound;
	
	//Player passed statuses
	// UPROPERTY(Replicated)
	// bool Player1Passed{ false };
	// UPROPERTY(Replicated)
	// bool Player2Passed{ false };

	//Widgets
	UPROPERTY()
	UTestUserWidget* GameInterface;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> Player1GameInterfaceWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> Player2GameInterfaceWidget;
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

	virtual void Tick(float DeltaSeconds) override;
	
	// Finds game interface
	void InitGameInterface();

	//
	void GetCardFromDeck(int32 PlayerNum);
	
	// // Multicast update of interface after changes in game state
	// UFUNCTION(NetMulticast, Reliable, WithValidation)
	// void Multi_UpdateInterface();
	// bool Multi_UpdateInterface_Validate();
	// void Multi_UpdateInterface_Implementation();

	// Multicast creating of round result widget 
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ShowRoundResult(int32 RoundWinner);
	bool Multi_ShowRoundResult_Validate(int32 RoundWinner);
	void Multi_ShowRoundResult_Implementation(int32 RoundWinner);

	// Multicast creating of match result widget 
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ShowMatchResult(int32 MatchWinner);
	bool Multi_ShowMatchResult_Validate(int32 MatchWinner);
	void Multi_ShowMatchResult_Implementation(int32 MatchWinner);

	// UFUNCTION()
	// void OnRep_PlayersTableScoreUpdated();

	UFUNCTION()
	void OnRep_Player1Updated();

	UFUNCTION()
	void OnRep_Player2Updated();
	
	UFUNCTION()
	void OnRep_ActivePlayerUpdated();

	// UFUNCTION()
	// void OnRep_PlayersRoundsScoresUpdated();
	//
	// UFUNCTION()
	// void OnRep_PlayersCardSlotsUpdated();
	
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

	// UFUNCTION(Server, Reliable, WithValidation)
	// void Server_ResetGame();
	// bool Server_ResetGame_Validate();
	// void Server_ResetGame_Implementation();
};

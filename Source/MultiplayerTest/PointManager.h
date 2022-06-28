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
	UPROPERTY(Replicated)
	int32 Player1Points;
	UPROPERTY(Replicated)
	int32 Player2Points;

	UPROPERTY(Replicated)
	TArray<int32> Player1CardSlots = {};
	UPROPERTY(Replicated)
	TArray<int32> Player2CardSlots = {};
	
	UPROPERTY()
	UTestUserWidget* GameInterface;

	UPROPERTY(Replicated)
	FString ActivePlayer;
public:	
	// Sets default values for this actor's properties
	APointManager();

	void IncreasePlayer1Points();
	void IncreasePlayer2Points();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void UpdateLabels();

	void InitGameInterface();
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_UpdateLabels();
	bool Multi_UpdateLabels_Validate();
	void Multi_UpdateLabels_Implementation();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_IncreasePlayer2Points();
	bool Server_IncreasePlayer2Points_Validate();
	void Server_IncreasePlayer2Points_Implementation();

	
	// UFUNCTION(NetMulticast, Reliable, WithValidation)
	// void Multi_IncreasePlayer2Points();
	// bool Multi_IncreasePlayer2Points_Validate();
	// void Multi_IncreasePlayer2Points_Implementation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EndTurn();

	void InitPlayers();
};

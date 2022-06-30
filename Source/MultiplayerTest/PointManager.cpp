// Fill out your copyright notice in the Description page of Project Settings.


#include "PointManager.h"

#include "TestCharacter.h"
#include "TestUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APointManager::APointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void APointManager::BeginPlay()
{
	Super::BeginPlay();

	//Init and update interface for players
	//IDK how to manage this w/o timers :(
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::InitGameInterface, 0.01);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.01);

	// Server randomly chooses first active player
	if(HasAuthority())
	{
		if (FMath::RandBool())
		{
			ActivePlayer = "Player1";
		}
		else
		{
			ActivePlayer = "Player2";
		}
	}
	
}


void APointManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APointManager, Player1CardSlots);
	DOREPLIFETIME(APointManager, Player2CardSlots);
	DOREPLIFETIME(APointManager, ActivePlayer);
	DOREPLIFETIME(APointManager, Player1Passed);
	DOREPLIFETIME(APointManager, Player2Passed);
	DOREPLIFETIME(APointManager, Player1TableScore);
	DOREPLIFETIME(APointManager, Player2TableScore);
}


void APointManager::EndTurn()
{
	// Initialize temporary variables 
	TArray<int32> PlayerCardSlots;
	int32 PlayerTableScore;
	FString AnotherPlayer;
	bool AnotherPlayerPassed;
	if (ActivePlayer == "Player1")
	{
		PlayerCardSlots = Player1CardSlots;
		PlayerTableScore = Player1TableScore;
		AnotherPlayer = "Player2";
		AnotherPlayerPassed = Player2Passed;
		
	}
	else
	{
		PlayerCardSlots = Player2CardSlots;
		PlayerTableScore = Player2TableScore;
		AnotherPlayer = "Player1";
		AnotherPlayerPassed = Player1Passed;
	}

	//Add card from deck to slot and recalculate PlayerTableScore
	if (PlayerCardSlots.Num() < 9 && PlayerTableScore < 20)
	{
		GetCardFromDeck(ActivePlayer);
		if (ActivePlayer == "Player1")
		{
			PlayerTableScore = Player1TableScore;
		}
		else
		{
			PlayerTableScore = Player2TableScore;
		}
	}
	// If player score >= 20 he automatically passes
	if (PlayerTableScore >= 20)
	{
		Pass();
	}
	//Turn goes to another player if he hasn't passed already
	else if (!AnotherPlayerPassed)
	{
		ActivePlayer = AnotherPlayer;
	}

	//Update players interfaces 
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.05);
	
}

void APointManager::Pass()
{
	if (ActivePlayer == "Player1")
	{
		UE_LOG(LogTemp, Warning, TEXT("Player1 passed"));
		Player1Passed = true;
		if (!Player2Passed)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Active player is Player2"));
			ActivePlayer = "Player2";
		}
		else
		{
			ActivePlayer = "None";
			EndRound();
		}
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.05);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player2 passed"));
		Player2Passed = true;
		if (!Player1Passed)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Active player is Player1"));
			ActivePlayer = "Player1";
		}
		else
		{
			ActivePlayer = "None";
			EndRound();
		}
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.05);
	}
}

void APointManager::EndRound()
{
	UE_LOG(LogTemp, Warning, TEXT("Round ended"));

	//TODO Change score

	//TODO Show winner
	
	//TODO After a short time restart round
	
}

bool APointManager::Multi_UpdateInterface_Validate()
{
	return true;
}

void APointManager::Multi_UpdateInterface_Implementation()
{
	if (!GameInterface)
	{
		InitGameInterface();
	}
	GameInterface->UpdatePlayerInterface(Player1CardSlots, Player2CardSlots, ActivePlayer,
		Player1TableScore, Player2TableScore);
}

void APointManager::InitGameInterface()
{
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UTestUserWidget::StaticClass());
	if (FoundWidgets.Num() > 0)
	{
		if (UTestUserWidget* Widget = Cast<UTestUserWidget>(FoundWidgets[0]))
		{
			GameInterface = Widget;
		}
	}
}

void APointManager::GetCardFromDeck(FString Player)
{
	//TODO implement player decks
	if (Player == "Player1")
	{
		const int32 CardValue = FMath::RandRange(1, 9);
		Player1TableScore += CardValue;
		Player1CardSlots.Add(CardValue);
		UE_LOG(LogTemp, Warning, TEXT("Player1 score: %d"), Player1TableScore);
	}
	else
	{
		const int32 CardValue = FMath::RandRange(1, 9);
		Player2TableScore += CardValue;
		Player2CardSlots.Add(CardValue);
		UE_LOG(LogTemp, Warning, TEXT("Player2 score: %d"), Player2TableScore);
	}
	
}

// old way of increasing points (for reference)
/*
void APointManager::IncreasePlayer1Points()
{
	if (Player1CardSlots.Num() < 9)
	{
		Player1CardSlots.Add(FMath::RandRange(1, 9));		
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateLabels, 0.05);

	
}

void APointManager::IncreasePlayer2Points()
{
	Server_IncreasePlayer2Points();	
}

bool APointManager::Server_IncreasePlayer2Points_Validate()
{
	return true;
}

void APointManager::Server_IncreasePlayer2Points_Implementation()
{
	if (Player2CardSlots.Num() < 9)
	{
		Player2CardSlots.Add(FMath::RandRange(1, 9));		
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateLabels, 0.05);

	
}
*/



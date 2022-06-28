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

	// Choose first active player
	//TODO: implement random generation
	if (HasAuthority())
	{
		ActivePlayer = "Player1";
	}	
}


void APointManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APointManager, Player1CardSlots);
	DOREPLIFETIME(APointManager, Player2CardSlots);
	DOREPLIFETIME(APointManager, ActivePlayer);
	
}


void APointManager::EndTurn()
{
	if (ActivePlayer == "Player1")
	{
		if (Player1CardSlots.Num() < 9)
		{
			//TODO: Choose card from pool
			Player1CardSlots.Add(FMath::RandRange(1, 9));		
		}
		
		ActivePlayer = "Player2";
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.05);
	}
	else
	{
		if (Player2CardSlots.Num() < 9)
		{
			//TODO: Choose card from pool
			Player2CardSlots.Add(FMath::RandRange(1, 9));		
		}

		ActivePlayer = "Player1";
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.05);
	}
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
	GameInterface->UpdatePlayerInterface(Player1CardSlots, Player2CardSlots, ActivePlayer);
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



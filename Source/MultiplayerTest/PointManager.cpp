// Fill out your copyright notice in the Description page of Project Settings.


#include "PointManager.h"

#include "TestUserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
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

	//IDK how to manage this w/o timers :(
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::InitGameInterface, 0.01);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateLabels, 0.01);
	
}


void APointManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APointManager, Player1Points);
	DOREPLIFETIME(APointManager, Player2Points);
	DOREPLIFETIME(APointManager, Player1CardSlots);
	DOREPLIFETIME(APointManager, Player2CardSlots);
	
}

// Called every frame
void APointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString DebugMsg1 = FString::Printf(TEXT("Player 1 score: %d"), Player1Points);
	FString DebugMsg2 = FString::Printf(TEXT("Player 2 score: %d"), Player2Points);
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, DebugMsg1);
	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, DebugMsg2);
}


void APointManager::IncreasePlayer1Points()
{
	Player1Points += 1;
	UE_LOG(LogTemp, Warning, TEXT("IncreasePlayer1Points %d"), Player1Points);

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
	Player2Points += 1;
	GameInterface->UpdatePointsCounters(Player1Points, Player2Points);

	if (Player2CardSlots.Num() < 9)
	{
		Player2CardSlots.Add(FMath::RandRange(1, 9));		
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateLabels, 0.05);

	
}

bool APointManager::Multi_UpdateLabels_Validate()
{
	return true;
}

void APointManager::Multi_UpdateLabels_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Multi_UpdateLabels_Implementation %d %d"), Player1Points, Player2Points);
	if (!GameInterface)
	{
		InitGameInterface();
	}
	GameInterface->UpdatePointsCounters(Player1Points, Player2Points);
	GameInterface->UpdateCardSlots(Player1CardSlots, Player2CardSlots);
}

void APointManager::InitGameInterface()
{
	// Finding game interface
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UTestUserWidget::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("Found %d widgets"), FoundWidgets.Num());
	if (FoundWidgets.Num() > 0)
	{
		
		if (UTestUserWidget* Widget = Cast<UTestUserWidget>(FoundWidgets[0]))
		{
			GameInterface = Widget;
		}
	}
}





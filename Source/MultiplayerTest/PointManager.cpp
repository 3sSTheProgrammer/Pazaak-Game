// Fill out your copyright notice in the Description page of Project Settings.


#include "PointManager.h"

#include "Engine/Engine.h"
#include "GameEndWidget.h"
#include "RoundEndWidget.h"
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

	// Server randomly chooses first active player
	if(HasAuthority())
	{
		if (FMath::RandBool())
		{
			ActivePlayer = "Player1";
			ActivePlayerInt = 1;
		}
		else
		{
			ActivePlayer = "Player2";
			ActivePlayerInt = 2;
		}
		PlayerBeganPreviousRound = ActivePlayer;
	}
}

// Called when the game starts or when spawned
void APointManager::BeginPlay()
{
	Super::BeginPlay();

	//Init and update interface for players
	//IDK how to manage this w/o timers :(

	//FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::InitGameInterface, 0.01);
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.01);

	UUserWidget* Widget;
	if (HasAuthority())
	{
		Widget = CreateWidget(GetWorld(), Player1GameInterfaceWidget);
	}
	else
	{
		Widget = CreateWidget(GetWorld(), Player2GameInterfaceWidget);
	}
	
	if (Widget)
	{
		GameInterface = Cast<UTestUserWidget>(Widget);
		if (GameInterface)
		{
			GameInterface->AddToViewport();
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);

			OnRep_ActivePlayerUpdated();
			//UE_LOG(LogTemp, Warning, TEXT("Game interface created"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: couldn't create interface widget"));
		}
	}
	
	// Server randomly chooses first active player
	// if(HasAuthority())
	// {
	// 	if (FMath::RandBool())
	// 	{
	// 		ActivePlayer = "Player1";
	// 	}
	// 	else
	// 	{
	// 		ActivePlayer = "Player2";
	// 	}
	// 	PlayerBeganPreviousRound = ActivePlayer;
	// }
	
	OnRep_ActivePlayerUpdated();
	OnRep_PlayersCardSlotsUpdated();
	OnRep_PlayersTableScoreUpdated();
	OnRep_PlayersRoundsScoresUpdated();
	// FTimerHandle SyncTimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(SyncTimerHandle, this, &APointManager::UpdateClientInterface, 1);
}

void APointManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green,
		FString::Printf(TEXT("ActivePlayer is %s"), *ActivePlayer));
	GEngine->AddOnScreenDebugMessage(1, DeltaSeconds, FColor::Red,
		FString::Printf(TEXT("ActivePlayer is %d"), ActivePlayerInt));
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
	DOREPLIFETIME(APointManager, Player1RoundsScore);
	DOREPLIFETIME(APointManager, Player2RoundsScore);
}


void APointManager::EndTurn()
{
	// Initialize temporary variables 
	TArray<int32> PlayerCardSlots;
	int32 PlayerTableScore;
	FString AnotherPlayer;
	bool AnotherPlayerPassed;

	int32 AnotherPlayerInt;

	if (ActivePlayer == "Player1")
	{
		PlayerCardSlots = Player1CardSlots;
		PlayerTableScore = Player1TableScore;
		AnotherPlayer = "Player2";
		AnotherPlayerPassed = Player2Passed;

		AnotherPlayerInt = 2;
	}
	else
	{
		PlayerCardSlots = Player2CardSlots;
		PlayerTableScore = Player2TableScore;
		AnotherPlayer = "Player1";
		AnotherPlayerPassed = Player1Passed;

		AnotherPlayerInt = 1;
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

	// else if (AnotherPlayerPassed)
	// {
	// 	if (PlayerTableScore > AnotherPlayerTableScore)
	// 	{
	// 		Pass();
	// 	}
	// }
	
	else if (!AnotherPlayerPassed)
	{
		ActivePlayer = AnotherPlayer;
		ActivePlayerInt = AnotherPlayerInt;
		OnRep_ActivePlayerUpdated();
	}

	
	
	
	//Update players interfaces 
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.15);
	
}

void APointManager::Pass()
{
	//Initialize temporary variables
	bool OtherPlayerPassed;
	FString OtherPlayer;
	int32 PlayerTableScore;

	int32 OtherPlayerInt;
	
	if (ActivePlayer == "Player1")
	{
		Player1Passed = true;
		PlayerTableScore = Player1TableScore;
		OtherPlayer = "Player2";
		OtherPlayerPassed = Player2Passed;

		OtherPlayerInt = 2;
	}
	else
	{
		Player2Passed = true;
		PlayerTableScore = Player2TableScore;
		OtherPlayer = "Player1";
		OtherPlayerPassed = Player1Passed;

		OtherPlayerInt = 1;
	}

	//If player score >= 20 or other player also passed round ends
	if (PlayerTableScore >= 20 || OtherPlayerPassed)
	{
		// ActivePlayer = "None";
		// OnRep_ActivePlayerUpdated();
		EndRound();
	}
	//Otherwise turn goes to other player 
	else
	{
		ActivePlayer = OtherPlayer;
		ActivePlayerInt = OtherPlayerInt;
		OnRep_ActivePlayerUpdated();
	}
	
	//Update players interfaces 
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.05);
}

void APointManager::EndRound()
{
	UE_LOG(LogTemp, Warning, TEXT("Round ended"));

	//Identify winner
	FString RoundWinner;
	if (Player1TableScore > 20)
	{
		RoundWinner = "Player2";
	}
	else if (Player2TableScore > 20)
	{
		RoundWinner = "Player1";
	}
	else if (Player1TableScore == 20)
	{
		RoundWinner = "Player1";
	}
	else if (Player2TableScore == 20)
	{
		RoundWinner = "Player2";
	}
	else if (Player1TableScore == Player2TableScore)
	{
		RoundWinner = "Draw";
	}
	else if (Player1TableScore > Player2TableScore)
	{
		RoundWinner = "Player1";
	}
	else
	{
		RoundWinner = "Player2";
	}

	UE_LOG(LogTemp, Warning, TEXT("Winner is %s"), *RoundWinner);

	//Change rounds score
	if (RoundWinner == "Player1")
	{
		Player1RoundsScore += 1;
		OnRep_PlayersRoundsScoresUpdated();
	}
	else if (RoundWinner == "Player2")
	{
		Player2RoundsScore += 1;
		OnRep_PlayersRoundsScoresUpdated();
	}
	
	

	if (Player1RoundsScore == 3 || Player2RoundsScore == 3)
	{
		FString MatchWinner;
		if (Player1RoundsScore == 3)
		{
			MatchWinner = "Player1";
		}
		else
		{
			MatchWinner = "Player2";
		}
		//End match
		Multi_ShowMatchResult(MatchWinner);
	}
	else
	{
		//Show winner
		Multi_ShowRoundResult(RoundWinner);
		
		//After a short time restart round
		FTimerHandle ResetTimerHandle;
		if (HasAuthority())
		{
			GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &APointManager::ResetGame, 3);
		}
		
		//ResetGame();
	}
	
	
}

void APointManager::ResetGame()
{
	Player1CardSlots = {};
	Player2CardSlots = {};
	Player1TableScore = 0;
	Player2TableScore = 0;
	Player1Passed = false;
	Player2Passed = false;
	
	// Remember who began previous round and change to another player
	if (PlayerBeganPreviousRound == "Player1")
	{
		ActivePlayer = "Player2";
	}
	else
	{
		ActivePlayer = "Player1";
	}
	PlayerBeganPreviousRound = ActivePlayer;
	
	//TODO TEST
	OnRep_PlayersTableScoreUpdated();
	OnRep_ActivePlayerUpdated();
	OnRep_PlayersRoundsScoresUpdated();
	OnRep_PlayersCardSlotsUpdated();
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APointManager::Multi_UpdateInterface, 0.15);
	
}

bool APointManager::Server_ResetGame_Validate()
{
	return true;
}

void APointManager::Server_ResetGame_Implementation()
{
	ResetGame();
}

// bool APointManager::Multi_UpdateInterface_Validate()
// {
// 	return true;
// }

// void APointManager::Multi_UpdateInterface_Implementation()
// {
// 	if (!GameInterface)
// 	{
// 		InitGameInterface();
// 	}
// 	GameInterface->UpdatePlayerInterface(Player1CardSlots, Player2CardSlots, ActivePlayer,
// 		Player1TableScore, Player2TableScore, Player1RoundsScore, Player2RoundsScore);
// }

bool APointManager::Multi_ShowRoundResult_Validate(const FString& RoundWinner)
{
	return true;
}

void APointManager::Multi_ShowRoundResult_Implementation(const FString& RoundWinner)
{
	UUserWidget* Widget = CreateWidget(GetWorld(), RoundEndWidget);
	if (URoundEndWidget* CreatedWidget = Cast<URoundEndWidget>(Widget))
	{
		CreatedWidget->SetRoundResult(RoundWinner);
		CreatedWidget->AddToViewport();
	}
}

bool APointManager::Multi_ShowMatchResult_Validate(const FString& RoundWinner)
{
	return true;
}

void APointManager::Multi_ShowMatchResult_Implementation(const FString& RoundWinner)
{
	UUserWidget* Widget = CreateWidget(GetWorld(), MatchEndWidget);
	if (UGameEndWidget* CreatedWidget = Cast<UGameEndWidget>(Widget))
	{
		CreatedWidget->SetMatchResult(RoundWinner);
		CreatedWidget->AddToViewport();
	}
}

void APointManager::OnRep_PlayersTableScoreUpdated()
{
	// if (!GameInterface)
	// {
	// 	InitGameInterface();
	// }
	GameInterface->UpdateTableScores(Player1TableScore, Player2TableScore);
}

void APointManager::OnRep_ActivePlayerUpdated()
{
	// if (!GameInterface)
	// {
	// 	InitGameInterface();
	// }
	// if (HasAuthority())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Server updating buttons"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Client updating buttons"));
	// }
	GameInterface->UpdateButtons(ActivePlayer);
}

void APointManager::OnRep_PlayersRoundsScoresUpdated()
{
	// if (!GameInterface)
	// {
	// 	InitGameInterface();
	// }
	GameInterface->UpdateRoundScores(Player1RoundsScore, Player2RoundsScore);
}

void APointManager::OnRep_PlayersCardSlotsUpdated()
{
	// if (!GameInterface)
	// {
	// 	InitGameInterface();
	// }
	GameInterface->UpdatePlayersCardSlots(Player1CardSlots, Player2CardSlots);
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
		OnRep_PlayersTableScoreUpdated();
		OnRep_PlayersCardSlotsUpdated();
		//UE_LOG(LogTemp, Warning, TEXT("Player1 score: %d"), Player1TableScore);
	}
	else
	{
		const int32 CardValue = FMath::RandRange(1, 9);
		Player2TableScore += CardValue;
		Player2CardSlots.Add(CardValue);
		OnRep_PlayersTableScoreUpdated();
		OnRep_PlayersCardSlotsUpdated();
		//UE_LOG(LogTemp, Warning, TEXT("Player2 score: %d"), Player2TableScore);
	}
}





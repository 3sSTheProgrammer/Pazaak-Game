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
	bAlwaysRelevant = true;
	
	// Server randomly chooses first active player
	// if(HasAuthority())
	// {
	// 	//ActivePlayer = 1;
	// 	if (FMath::RandBool())
	// 	{
	// 		ActivePlayer = 1;
	// 	}
	// 	else
	// 	{
	// 		ActivePlayer = 2;
	// 	}
	// 	PlayerBeganPreviousRound = ActivePlayer;
	// }

	if (HasAuthority())
	{
		Player1.ID = EPlayerEnum::Player1;
		Player2.ID = EPlayerEnum::Player2;
		
		//Creating 
		PlayerMap.Add(EPlayerEnum::Player1, &Player1);
		PlayerMap.Add(EPlayerEnum::Player2, &Player2);

		// Server randomly chooses first active player
		if (FMath::RandBool())
		{
			ActivePlayerNum = EPlayerEnum::Player1;
		}
		else
		{
			ActivePlayerNum = EPlayerEnum::Player2;
		}
		PlayerBeganPreviousRound = ActivePlayerNum;
	}
	
	
	//UE_LOG(LogTemp, Warning, TEXT("TEST %d"), TestActivePlayer.PlayerPassed);
	
}

// Called when the game starts or when spawned
void APointManager::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play"));
	
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
	// 		ActivePlayer = 1;
	// 	}
	// 	else
	// 	{
	// 		ActivePlayer = 2;
	// 	}
	// 	PlayerBeganPreviousRound = ActivePlayer;
	// }
	
	OnRep_ActivePlayerUpdated();
	OnRep_Player1Updated();
	OnRep_Player2Updated();
	

	// OnRep_Player1Updated();
	// OnRep_Player2Updated();	
	
	
	// FTimerHandle SyncTimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(SyncTimerHandle, this, &APointManager::UpdateClientInterface, 1);
}

void APointManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FColor MessageColor;
	if (HasAuthority())
	{
		MessageColor = FColor::Green;
	}
	else
	{
		MessageColor = FColor::Red;
	}
	GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, MessageColor,
		FString::Printf(TEXT("I am player %d; ActivePlayer is %d"), !HasAuthority(), ActivePlayerNum));
}


void APointManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(APointManager, Player1CardSlots);
	// DOREPLIFETIME(APointManager, Player2CardSlots);
	// DOREPLIFETIME(APointManager, ActivePlayer);
	// DOREPLIFETIME(APointManager, Player1Passed);
	// DOREPLIFETIME(APointManager, Player2Passed);
	// DOREPLIFETIME(APointManager, Player1TableScore);
	// DOREPLIFETIME(APointManager, Player2TableScore);
	// DOREPLIFETIME(APointManager, Player1RoundsScore);
	// DOREPLIFETIME(APointManager, Player2RoundsScore);

	DOREPLIFETIME(APointManager, Player1);
	DOREPLIFETIME(APointManager, Player2);
	DOREPLIFETIME(APointManager, ActivePlayerNum);
}


void APointManager::EndTurn()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("End turn called from client!"));
		return;
	}

	// Taking a card
	GetCardFromDeck(ActivePlayerNum);

	// If at least one pass condition is met, pass 
	if (PlayerMap[ActivePlayerNum]->CardSlots.Num() == 9 || PlayerMap[ActivePlayerNum]->TableScore >= 20)
	{
		Pass();
		return;
	}
	
	// If another player hasn't passed change active player
	// (1 - ActivePlayerNum) is NonActivePlayerNum
	if (!(PlayerMap[1 - ActivePlayerNum]->Passed))
	{
		ActivePlayerNum = 1 - ActivePlayerNum;
		OnRep_ActivePlayerUpdated();
	}
}

void APointManager::Pass()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pass called from client!"));
		return;
	}

	PlayerMap[ActivePlayerNum]->Passed = true;
	
	// TODO mb create function bool NotActivePlayerPassed() to check condition
	// TODO or/and function void ChangeActivePlayer()
	// if another player has passed or active player score is more then 20
	if (PlayerMap[1 - ActivePlayerNum]->Passed ||
		PlayerMap[ActivePlayerNum]->TableScore > 20)
	{
		EndRound();
	}
	else
	{		
		// change active player
		ActivePlayerNum = 1 - ActivePlayerNum;
		OnRep_ActivePlayerUpdated();
	}
}

void APointManager::EndRound()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("EndRound called from client!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Round ended"));

	// Disable buttons for both players
	ActivePlayerNum = EPlayerEnum::PlayerNone;
	
	//Change players round scores
	int32 RoundWinner{ EPlayerEnum::PlayerNone };
	if (Player1.TableScore > 20)
	{
		RoundWinner = EPlayerEnum::Player2;
	}
	else if (Player2.TableScore > 20)
	{
		RoundWinner = EPlayerEnum::Player1;
	}
	else if (Player1.TableScore > Player2.TableScore)
	{
		RoundWinner = EPlayerEnum::Player1;
	}
	else if (Player2.TableScore > Player1.TableScore)
	{
		RoundWinner = EPlayerEnum::Player2;
	}

	// If round wasn't a draw 
	if (RoundWinner != EPlayerEnum::PlayerNone)
	{
		// Increase winner's round points
        PlayerMap[RoundWinner]->RoundsScore += 1;
		OnRep_Player1Updated();
		OnRep_Player2Updated();
		
		UE_LOG(LogTemp, Warning, TEXT("Round score is %d : %d"), Player1.TableScore, Player2.TableScore);
		
		// If one of players got 3 round wins
		if (Player1.RoundsScore == 3 || Player2.RoundsScore == 3)
		{
			//End match after a short time (needed for replication)
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,
				&APointManager::Multi_ShowMatchResult, RoundWinner);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.2, false);
		}
		else
		{
			//Show round winner
			Multi_ShowRoundResult(RoundWinner);
		
			// Restart round after delay
			FTimerHandle ResetTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &APointManager::ResetGame, 3);
		}
	}
}

void APointManager::ResetGame()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetGame called from client!"));
		return;
	}
	
	Player1.CardSlots = {};
	Player2.CardSlots = {};
	Player1.TableScore = 0;
	Player2.TableScore = 0;
	Player1.Passed = false;
	Player2.Passed = false;
	
	// Remember who began previous round and change to another player
	ActivePlayerNum = 1 - PlayerBeganPreviousRound;
	PlayerBeganPreviousRound = ActivePlayerNum;

	//Update interface on server
	OnRep_Player1Updated();
	OnRep_Player2Updated();
	OnRep_ActivePlayerUpdated();
}

// bool APointManager::Server_ResetGame_Validate()
// {
// 	return true;
// }
//
// void APointManager::Server_ResetGame_Implementation()
// {
// 	ResetGame();
// }

// bool APointManager::Multi_UpdateInterface_Validate()
// {
// 	return true;
// }
//
// void APointManager::Multi_UpdateInterface_Implementation()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("%d updating interface"), !HasAuthority());
// 	if (!GameInterface)
// 	{
// 		InitGameInterface();
// 	}
// 	GameInterface->UpdateButtons(ActivePlayerNum);
// }

bool APointManager::Multi_ShowRoundResult_Validate(int32 RoundWinner)
{
	if (!PlayerMap[RoundWinner])
	{
		return false;
	}

	if (PlayerMap[RoundWinner]->RoundsScore > 3)
	{
		return false;
	}
		
	return true;
}

void APointManager::Multi_ShowRoundResult_Implementation(int32 RoundWinner)
{
	UUserWidget* Widget = CreateWidget(GetWorld(), RoundEndWidget);
	if (URoundEndWidget* CreatedWidget = Cast<URoundEndWidget>(Widget))
	{
		CreatedWidget->SetRoundResult(RoundWinner);
		CreatedWidget->AddToViewport();
	}
}

bool APointManager::Multi_ShowMatchResult_Validate(int32 MatchWinner)
{
	return true;
}

void APointManager::Multi_ShowMatchResult_Implementation(int32 MatchWinner)
{
	UUserWidget* Widget = CreateWidget(GetWorld(), MatchEndWidget);
	if (UGameEndWidget* CreatedWidget = Cast<UGameEndWidget>(Widget))
	{
		CreatedWidget->SetMatchResult(MatchWinner);
		CreatedWidget->AddToViewport();
	}
}

void APointManager::OnRep_Player1Updated()
{
	if (!GameInterface)
	{
		InitGameInterface();
	}
	GameInterface->UpdatePlayerInterface(Player1);
}

void APointManager::OnRep_Player2Updated()
{
	if (!GameInterface)
	{
		InitGameInterface();
	}
	GameInterface->UpdatePlayerInterface(Player2);
}

void APointManager::OnRep_ActivePlayerUpdated()
{
	if (!GameInterface)
	{
		InitGameInterface();
	}
	GameInterface->UpdateButtons(ActivePlayerNum);
}

// void APointManager::OnRep_PlayersCardSlotsUpdated()
// {
// 	GameInterface->UpdatePlayersCardSlots(Player1CardSlots, Player2CardSlots);
// }

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

void APointManager::GetCardFromDeck(int32 PlayerNum)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCardFromDeck called from client!"));
		return;
	}

	//TODO implement player decks
	const int32 CardValue = FMath::RandRange(1, 9);
	PlayerMap[PlayerNum]->CardSlots.Add(CardValue);
	PlayerMap[PlayerNum]->TableScore += CardValue;

	if (PlayerNum == EPlayerEnum::Player1)
	{
		OnRep_Player1Updated();
	}
	else if (PlayerNum == EPlayerEnum::Player2)
	{
		OnRep_Player2Updated();
	}
}





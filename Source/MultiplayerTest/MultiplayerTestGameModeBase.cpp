// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerTestGameModeBase.h"

AMultiplayerTestGameModeBase::AMultiplayerTestGameModeBase()
{
	ConstructorHelpers::FObjectFinder<UClass>
		ControllerClass(TEXT("Class'/Game/MultiplayerTest/Blueprints/BP_PazaakPlayerController.BP_PazaakPlayerController_C'"));
	if (ControllerClass.Object != nullptr)
	{
		PlayerControllerClass = ControllerClass.Object;
	}
}

void AMultiplayerTestGameModeBase::PreLogin(const FString& Options, const FString& Address,
	const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (GetNumPlayers() == 2)
	{
		ErrorMessage = "Game is full";
	}
}

void AMultiplayerTestGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UE_LOG(LogTemp, Warning, TEXT("Player connected. Total players connected: %d"), GetNumPlayers());

	//Can I use StartPlay that way????
	if (GetNumPlayers() == 2)
	{
		StartPlay();
	}
}

void AMultiplayerTestGameModeBase::StartPlay()
{
	if (GetNumPlayers() == 2)
	{
		Super::StartPlay();
	}
	
}

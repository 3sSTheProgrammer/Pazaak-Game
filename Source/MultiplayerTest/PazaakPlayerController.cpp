// Fill out your copyright notice in the Description page of Project Settings.


#include "PazaakPlayerController.h"

#include "PointManager.h"
#include "TestUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void APazaakPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	InitPointManager();
}

bool APazaakPlayerController::Server_EndTurn_Validate()
{
	return true;
}

void APazaakPlayerController::Server_EndTurn_Implementation()
{
	if (!PointManager)
	{
		InitPointManager();
	}
	PointManager->EndTurn();
}

void APazaakPlayerController::InitPointManager()
{
	TArray<AActor*> PointManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointManager::StaticClass(),PointManagers);
	if (PointManagers.Num() > 0)
	{
		PointManager = Cast<APointManager>(PointManagers[0]);
	}
}

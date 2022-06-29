// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PazaakPlayerController.generated.h"

class UTestUserWidget;
class APointManager;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API APazaakPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	APointManager* PointManager;
	
public:

	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndTurn();
	bool Server_EndTurn_Validate();
	void Server_EndTurn_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Pass();
	bool Server_Pass_Validate();
	void Server_Pass_Implementation();

protected:
	//Finds point manager in the world
	void InitPointManager();
};

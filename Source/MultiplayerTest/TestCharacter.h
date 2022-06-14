// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

class ATestActor;
class UTestUserWidget;
class APointManager;

UCLASS()
class MULTIPLAYERTEST_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UTestUserWidget* GameInterface;

	UPROPERTY()
	APointManager* PointManager;
public:
	// Sets default values for this character's properties
	ATestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void IncreasePoints();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(APointManager* Manager);
	bool Server_Interact_Validate(APointManager* Manager);
	void Server_Interact_Implementation(APointManager* Manager);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class MULTIPLAYERTEST_API ATestActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	int ReplicatedParam = 1;
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ParamChange();
	bool Server_ParamChange_Validate();
	void Server_ParamChange_Implementation();
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
    void Multi_ParamChange();
    bool Multi_ParamChange_Validate();
    void Multi_ParamChange_Implementation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseParamChange();
	


public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

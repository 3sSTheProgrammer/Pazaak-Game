// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	// find static mesh component
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents(StaticMeshComponents);
	if (StaticMeshComponents.Num() > 0)
	{
		StaticMeshComponent = StaticMeshComponents[0];
	}
}

bool ATestActor::Server_ParamChange_Validate()
{
	return true;
}

void ATestActor::Server_ParamChange_Implementation()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Server_ParamChange_Implementation"));
	ReplicatedParam *= -1;
	Multi_ParamChange();
}

bool ATestActor::Multi_ParamChange_Validate()
{
	return true;
}

void ATestActor::Multi_ParamChange_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Multi_ParamChange_Implementation"));
	
	
		//UE_LOG(LogTemp, Warning, TEXT("ReplicatedParam %d"), ReplicatedParam);
	if (StaticMeshComponent)
	{
		FVector CurrentScale = GetActorScale3D();
			
		CurrentScale += FVector::OneVector * ReplicatedParam;
		SetActorScale3D(CurrentScale);
	}
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString DebugMsg = FString::Printf(TEXT("Replicated param is %d"), ReplicatedParam);
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, DebugMsg);
}

void ATestActor::UseParamChange()
{
	if (HasAuthority())
	{
		ReplicatedParam *= -1;
		Multi_ParamChange(); //from the server
	}
	else
	{
		Server_ParamChange(); //from the client
	}
}


void ATestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestActor, ReplicatedParam);
	
}

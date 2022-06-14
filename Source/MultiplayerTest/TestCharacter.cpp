// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"

#include "PointManager.h"
#include "TestActor.h"
#include "TestUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;
	

}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Finding game interface
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UTestUserWidget::StaticClass());
	if (FoundWidgets.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d widgets"), FoundWidgets.Num());
		if (UTestUserWidget* Widget = Cast<UTestUserWidget>(FoundWidgets[0]))
		{
			GameInterface = Widget;
		}
		
	}

	// Finding points manager
	TArray<AActor*> FoundPointManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointManager::StaticClass(), FoundPointManagers);
	if (FoundPointManagers.Num() > 0)
	{
		if (APointManager* FoundPointManager = Cast<APointManager>(FoundPointManagers[0]))
		{
			PointManager = FoundPointManager;
		}
	}
}

void ATestCharacter::IncreasePoints()
{
	if (HasAuthority())
	{
		PointManager->IncreasePlayer1Points();
	}
	else
	{
		Server_Interact(PointManager);
	}
}

bool ATestCharacter::Server_Interact_Validate(APointManager* Manager)
{
	return true;
}

void ATestCharacter::Server_Interact_Implementation(APointManager* Manager)
{
	if (Manager)
	{
		Manager->IncreasePlayer2Points();
	}
}


// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAction( "ChangeActorSize", EInputEvent::IE_Pressed, this, &ATestCharacter::IncreasePoints);
	}
	
}


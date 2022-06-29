// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"

#include "PointManager.h"
#include "TestUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		//TODO Bind player actions
		// Pause - Escape
		// End turn - Space
		// Pass - Enter
		//PlayerInputComponent->BindAction( "ChangeActorSize", EInputEvent::IE_Pressed, this, &ATestCharacter::IncreasePoints);
	}
	
}


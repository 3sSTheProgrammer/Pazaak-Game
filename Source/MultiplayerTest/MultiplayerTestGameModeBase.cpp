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

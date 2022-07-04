// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API AMultiplayerTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerTestGameModeBase();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void StartPlay() override;
};

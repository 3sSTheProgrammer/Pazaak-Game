// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundEndWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API URoundEndWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TBRoundResult;
	
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetRoundResult(FString RoundWinner);
	
};

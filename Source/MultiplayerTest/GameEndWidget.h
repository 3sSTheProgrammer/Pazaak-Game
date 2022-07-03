// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TBMatchResult;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonPlayAgain;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonQuit;

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetMatchResult(int32 MatchWinner);
	
};

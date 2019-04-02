// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiceFromScratchGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DICEFROMSCRATCH_API ADiceFromScratchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADiceFromScratchGameModeBase();

	bool CheckPlayerNameExists(const FString& NewPlayerName);

	void NotifyPlayerJoined(const FString& NewPlayerName);

	void NotifyPlayerReady(const FString& NewPlayerName);

	void SendPlayerBet();

	void TryToChangeNumDice(int difference);

	void TryToChangeFaceValue(int difference);

	void CallBluff();

	void CallSpotOn();

	void StartGame();

private:
	void BluffCalled(bool success);

	void SpotOnCalled(bool success);

	FString GameOverCheck();

protected:
	virtual void BeginPlay() override;
};

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Player/PlayerPlatform.h"
#include "Complain.h"
#include "TowersGameMode.generated.h"

class ATowersPlayerController;
class ASpawnPad;
class AGoal;
class ATower;

#define PAYOUTDELAY 200

UCLASS(minimalapi)
class ATowersGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATowersGameMode();
	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void RequestSpawn(int playerIndex);
	void RequestFireProjectile(ATower* tower);
	ASpawnPad* PlaceSpawnPoint(AController* player, FVector spawnLocation, FRotator rotator);
	AGoal* PlaceGoal(FVector spawnLocation, FRotator rotator);
	ATower* PlaceTower(FVector spawnLocation, FRotator rotator);
	void PlacePlayerPlatform(ATowersPlayerController* player, int playerNumber);
	void RequestUpgrade(int playerIndex, int upgradeIndex);
	void RequestReward(int playerIndex, int reward);

	void NotifyHPChange(int playerIndex, int amount);

private:
	APlayerStart* player1StartPoint;
	APlayerStart* player2StartPoint;

	TArray<APlayerPlatform*> platforms;

	bool player1Spawned;
	bool player2Spawned;

	int numPlayers;

	int payoutTimer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};




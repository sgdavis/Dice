// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TowersPlayerStats.generated.h"

class APlayerPlatform;

enum STATUPGRADE
{
	LaneHitPoints = 0,
	LaneSlow = 1,
	LaneTimer = 2,

	MinionHitPoints = 3,
	MinionDamage = 4,
	MinionSpeed = 5,

	TowerDamage = 6,
	TowerShotSpeed = 7,
	TowerFiringSpeed = 8,

	MaxUpgrades = 9
};

USTRUCT()
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY()
		APlayerPlatform* platform;

	UPROPERTY()
		int playerIndex;
	UPROPERTY()
		int laneHitPoints;
	UPROPERTY()
		int income;
	UPROPERTY()
		int cash;
	UPROPERTY()
		int spawnCost;

	UPROPERTY()
		int upgrades[9];
	UPROPERTY()
		int costs[9];
};

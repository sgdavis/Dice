// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerGameStateBase.h"
#include "Player/PlayerPlatform.h"

void ATowerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATowerGameState, PlayerList);
}

void ATowerGameState::OnRep_PlayerList()
{
	PlayerListUpdatedCallback.ExecuteIfBound(PlayerList);
}

void ATowerGameState::RegisterPlayerListListener(FSGPlayerListUpdatedDelegate Callback)
{
	check(GetNetMode() != NM_DedicatedServer);

	PlayerListUpdatedCallback = Callback;
	Callback.ExecuteIfBound(PlayerList);
}

void ATowerGameState::AddPlayer(int index, APlayerPlatform* platform)
{
	FPlayerStats newPlayer;

	newPlayer.laneHitPoints = 5;
	newPlayer.income = 1;
	newPlayer.cash = 5;
	newPlayer.spawnCost = 5;
	for (int i = 0; i < STATUPGRADE::MaxUpgrades; i++)
	{
		newPlayer.upgrades[i] = 0;
	}
	for (int i = 0; i < STATUPGRADE::MaxUpgrades; i++)
	{
		newPlayer.costs[i] = 1;
	}
	newPlayer.platform = platform;
	newPlayer.playerIndex = index;

	PlayerList.Add(newPlayer);
}

FPlayerStats ATowerGameState::GetPlayerStatsCopy(int index)
{
	FPlayerStats ret;

	ret.laneHitPoints = PlayerList[index].laneHitPoints;
	ret.income = PlayerList[index].income;
	ret.cash = PlayerList[index].cash;
	ret.spawnCost = PlayerList[index].spawnCost;
	for (int i = 0; i < STATUPGRADE::MaxUpgrades; i++)
	{
		ret.upgrades[i] = PlayerList[index].upgrades[i];
	}
	for (int i = 0; i < STATUPGRADE::MaxUpgrades; i++)
	{
		ret.costs[i] = PlayerList[index].costs[i];
	}
	ret.platform = PlayerList[index].platform;
	ret.playerIndex = PlayerList[index].playerIndex;

	return ret;
}

void ATowerGameState::ReplacePlayersStats(int index, FPlayerStats newStats)
{
	PlayerList.RemoveAt(index);
	PlayerList.EmplaceAt(index, newStats);
}

APlayerPlatform* ATowerGameState::NotifyHPChange(int playerIndex, int amount)
{
	APlayerPlatform* ret = NULL;

	int i = 0;
	bool destroy = false;
	for (i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i].playerIndex == playerIndex)
		{
			FPlayerStats tempStats = GetPlayerStatsCopy(i);

			tempStats.laneHitPoints = tempStats.laneHitPoints + amount;
			if (tempStats.laneHitPoints == 0)
			{
				destroy = true;
			}
			ReplacePlayersStats(i, tempStats);
			break;
		}
	}

	if (destroy)
	{
		ret = PlayerList[i].platform;
	}

	return ret;
}

void ATowerGameState::NotifyUpgrade(int playerIndex, int upgradeIndex, int amount, float costScale)
{
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i].playerIndex == playerIndex)
		{
			FPlayerStats tempStats = GetPlayerStatsCopy(i);

			tempStats.upgrades[upgradeIndex] = tempStats.upgrades[upgradeIndex] + amount;
			tempStats.costs[upgradeIndex] = FMath::CeilToInt(tempStats.costs[upgradeIndex] * costScale);

			ReplacePlayersStats(i, tempStats);
			break;
		}
	}
}

void ATowerGameState::NotifyPayouts()
{
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		FPlayerStats tempStats = GetPlayerStatsCopy(i);

		tempStats.cash = tempStats.cash + tempStats.income;

		ReplacePlayersStats(i, tempStats);
	}
}

void ATowerGameState::ModifySpawnCost(int playerIndex, int amount)
{
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i].playerIndex == playerIndex)
		{
			FPlayerStats tempStats = GetPlayerStatsCopy(i);

			tempStats.spawnCost = tempStats.spawnCost + amount;

			ReplacePlayersStats(i, tempStats);
			break;
		}
	}
}

void ATowerGameState::ChargePlayer(int playerIndex, int amount)
{
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i].playerIndex == playerIndex)
		{
			FPlayerStats tempStats = GetPlayerStatsCopy(i);

			tempStats.cash = tempStats.cash - amount;

			ReplacePlayersStats(i, tempStats);
			break;
		}
	}
}

void ATowerGameState::ModifyIncome(int playerIndex, int amount)
{
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i].playerIndex == playerIndex)
		{
			FPlayerStats tempStats = GetPlayerStatsCopy(i);

			tempStats.income = tempStats.income + amount;

			ReplacePlayersStats(i, tempStats);
			break;
		}
	}
}

int ATowerGameState::GetNumPlayersAlive()
{
	int ret = 0;

	for (int i = 0; i < PlayerList.Num(); i++)
	{
		if (PlayerList[i].laneHitPoints > 0)
		{
			ret++;
		}
	}

	return ret;
}
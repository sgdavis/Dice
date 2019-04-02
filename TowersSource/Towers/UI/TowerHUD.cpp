// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerHUD.h"
#include "Player/TowersPlayerController.h"
#include "Game/TowersPlayerStats.h"

UTowerHUD::UTowerHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	pPlayerController = NULL;
}

void UTowerHUD::OnLaneHPButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::LaneHitPoints);
}
void UTowerHUD::OnLaneSlowButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::LaneSlow);
}
void UTowerHUD::OnLaneSpawnButtonClicked()
{
	pPlayerController->RequestSpawn();
}

void UTowerHUD::OnMinionHPButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::MinionHitPoints);
}
void UTowerHUD::OnMinionSpeedButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::MinionSpeed);
}
void UTowerHUD::OnMinionDamageButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::MinionDamage);
}

void UTowerHUD::OnTowerDamageButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::TowerDamage);
}
void UTowerHUD::OnTowerShotSpeedButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::TowerShotSpeed);
}
void UTowerHUD::OnTowerSpeedButtonClicked()
{
	pPlayerController->RequestUpgrade(STATUPGRADE::TowerFiringSpeed);
}

void UTowerHUD::UpdatePlayerList(const TArray<FPlayerStats>& players)
{
	CLIENTCOMPLAINT(TEXT("TRYING TO UPDATE UI"));

	for (FPlayerStats stats : players)
	{
		if (stats.playerIndex == pPlayerController->GetPlayerIndex())
		{
			if (HPLabel != NULL)
			{
				HPLabel->SetText(FText::FromString(FString::Printf(TEXT("HP: %d"), stats.laneHitPoints)));
			}
			if (GoldLabel != NULL)
			{
				GoldLabel->SetText(FText::FromString(FString::Printf(TEXT("Cash: %d"), stats.cash)));
			}
			if (IncomeLabel != NULL)
			{
				IncomeLabel->SetText(FText::FromString(FString::Printf(TEXT("Income: %d"), stats.income)));
			}

			if (MinionDamageUpLabel != NULL)
			{
				MinionDamageUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::MinionDamage], stats.costs[STATUPGRADE::MinionDamage])));
			}
			if (MinionHPUpLabel != NULL)
			{
				MinionHPUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::MinionHitPoints], stats.costs[STATUPGRADE::MinionHitPoints])));
			}
			if (MinionSpeedUpLabel != NULL)
			{
				MinionSpeedUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::MinionSpeed], stats.costs[STATUPGRADE::MinionSpeed])));
			}

			if (TowerDamageUpLabel != NULL)
			{
				TowerDamageUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::TowerDamage], stats.costs[STATUPGRADE::TowerDamage])));
			}
			if (TowerShotSpeedUpLabel != NULL)
			{
				TowerShotSpeedUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::TowerShotSpeed], stats.costs[STATUPGRADE::TowerShotSpeed])));
			}
			if (TowerSpeedUpLabel != NULL)
			{
				TowerSpeedUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::TowerFiringSpeed], stats.costs[STATUPGRADE::TowerFiringSpeed])));
			}

			if (LaneHPUpLabel != NULL)
			{
				LaneHPUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::LaneHitPoints], stats.costs[STATUPGRADE::LaneHitPoints])));
			}
			if (LaneSlowUpLabel != NULL)
			{
				LaneSlowUpLabel->SetText(FText::FromString(FString::Printf(TEXT("Current: %d Cost: %d"), stats.upgrades[STATUPGRADE::LaneSlow], stats.costs[STATUPGRADE::LaneSlow])));
			}
			if (LaneSpawnLabel != NULL)
			{
				LaneSpawnLabel->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), stats.spawnCost)));
			}

			break;
		}
	}
}
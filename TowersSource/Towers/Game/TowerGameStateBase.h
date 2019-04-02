// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Game/TowersPlayerStats.h"
#include "Net/UnrealNetwork.h"
#include "Player/TowersPlayerController.h"
#include "TowerGameStateBase.generated.h"

class APlayerPlatform;

DECLARE_DELEGATE_OneParam(FSGPlayerListUpdatedDelegate, const TArray<FPlayerStats>&);

UCLASS()
class TOWERS_API ATowerGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerList)
	TArray<FPlayerStats> PlayerList;

	TArray<FPlayerStats> GetPlayerList() { return PlayerList; }

	UFUNCTION()
		void AddPlayer(int index, APlayerPlatform* platform);

	UFUNCTION()
		void OnRep_PlayerList();

	FSGPlayerListUpdatedDelegate PlayerListUpdatedCallback;

	void RegisterPlayerListListener(FSGPlayerListUpdatedDelegate Callback);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATowersPlayerController* pPlayerController;

	void SetLocalPlayerController(ATowersPlayerController* player) { pPlayerController = player; }

	FPlayerStats GetPlayerStatsCopy(int index);
	void ReplacePlayersStats(int index, FPlayerStats newStats);

	int GetNumPlayersAlive();

	APlayerPlatform* NotifyHPChange(int playerIndex, int amount);
	void NotifyUpgrade(int playerIndex, int upgradeIndex, int amount, float costMultiplier);
	void NotifyPayouts();
	void ModifySpawnCost(int playerIndex, int amount);
	void ChargePlayer(int playerIndex, int amount);
	void ModifyIncome(int playerIndex, int amount);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/TowersPlayerController.h"
#include "Minion/SpawnPad.h"
#include "Complain.h"
#include "PlayerPlatform.generated.h"

class AGoal;
class ATower;
class AMinion;

UCLASS()
class TOWERS_API APlayerPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerPlatform();

	void SetPlatformPlayer(ATowersPlayerController* player);
	ATowersPlayerController* GetPlatformPlayer();
	ASpawnPad* GetSpawner();
	AGoal* GetGoal();
	ATower* GetTower();

	FVector GetEnemyLocation();
	FRotator GetEnemyRotator();

	UFUNCTION(Server, Reliable, WithValidation)
	void OnDamageTaken(int amount);

	void AddMinion(AMinion* minion);
	void RemoveMinion(AMinion* minion, bool killed);
	AMinion* GetClosestMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ATowersPlayerController* pMyPlayer;

	bool bPlayerTeleported;
	void TeleportPlayer();

	UStaticMeshComponent* pPlayerSpawn;
	UStaticMeshComponent* pEnemySpawn;
	UStaticMeshComponent* pPadSpawn;
	UStaticMeshComponent* pGoalSpawn;
	UStaticMeshComponent* pTowerSpawn;
	ASpawnPad* pSpawnPad;
	AGoal* pGoal;
	ATower* pTower;

	TArray<AMinion*> minions;
	void CleanMinions();

	bool bHasRegisteredFireRateCallback;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

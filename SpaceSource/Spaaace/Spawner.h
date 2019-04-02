// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Complain.h"
#include "Spawner.generated.h"

UCLASS()
class SPAAACE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void StopSpawning() { waveNumber = 0; aiSpawnTimer = -1; numSpawned = 0; }
    void SpawnWave(int number) { waveNumber = number; aiSpawnTimer = 0; numSpawned = 0; }
    void SpawnShip();

    USplineComponent* pSpawnPath;
    FVector spawnPoint;
    FRotator spawnRotation;

    UClass* pAIShipClass;
    void SetShipType(UClass* shipType) { pAIShipClass = shipType; }

    AActor* pPlayerShip;
    void SetPlayerShip(AActor* ship) { pPlayerShip = ship; }

    float aiSpawnTimer;
    int aiSpawnDelay;

    int waveNumber;
    int numSpawned;

};

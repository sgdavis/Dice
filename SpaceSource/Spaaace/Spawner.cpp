// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "AIShip.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    pSpawnPath = NULL;
    pAIShipClass = NULL;

    pPlayerShip = NULL;

    aiSpawnTimer = -1;
    aiSpawnDelay = 2;

    waveNumber = 5;
    numSpawned = 0;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

    TArray<USplineComponent*> allSplines;
    GetComponents<USplineComponent>(allSplines);

    FString meshName;
    for (int i = 0; i < allSplines.Num(); i++)
    {
        meshName = allSplines[i]->GetFName().ToString();
        if (meshName.Compare("First") == 0)
        {
            pSpawnPath = allSplines[i];
        }
    }	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (aiSpawnTimer == 0)
    {
        CLIENTCOMPLAINT("SPAWNING SHIP");
        SpawnShip();
    }
    else if (aiSpawnTimer > 0)
    {
        aiSpawnTimer = FMath::Max(0.0f, aiSpawnTimer - DeltaTime);
    }
}

void ASpawner::SpawnShip()
{

    if (pAIShipClass == NULL)
    {
        CLIENTCOMPLAINT("CANT SPAWN WITHOUT A SHIP");
    }

    UWorld* pWorld = GetWorld();
    if (pWorld)
    {
        FActorSpawnParameters parameters;
        parameters.Owner = this;

        AAIShip* newShip = pWorld->SpawnActor<AAIShip>(pAIShipClass, spawnPoint, spawnRotation);
        newShip->SetFollowSpline(pSpawnPath);
        newShip->SetTarget(pPlayerShip);

        if (++numSpawned == waveNumber)
        {
            StopSpawning();
        }
        else
        {
            aiSpawnTimer = aiSpawnDelay;
        }
    }
}

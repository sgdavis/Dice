// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Spawner.h"
#include "SpaaaceGameMode.generated.h"

UCLASS(minimalapi)
class ASpaaaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaaaceGameMode();

    virtual void Tick(float DeltaTime) override;

    ASpawner* pTopRightSwingSpawner;
    ASpawner* pTopLeftSwingSpawner;
    ASpawner* pLeftStrafeSpawner;
    ASpawner* pRightStrafeSpawner;

    AActor* pPlayerShip;
    FVector playersStartLocation;
    UClass* pAIShipClass;

    float GameTimer;
    int GameSteps;
    int PlayerPoints;
    int PlayerHighScore;

    void HandleGameSteps(float DeltaTime);

    void PlayerWasHit();

    void PlayerKilledMe(int numPoints, FVector location);
};




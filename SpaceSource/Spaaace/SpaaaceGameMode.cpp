// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SpaaaceGameMode.h"
#include "SpaaacePlayerController.h"
#include "SpaaaceCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "AIShip.h"
#include "Ship/Ship.h"
#include "Complain.h"

ASpaaaceGameMode::ASpaaaceGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASpaaacePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Ship_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    PrimaryActorTick.bStartWithTickEnabled = true;
    PrimaryActorTick.bCanEverTick = true;

    pTopRightSwingSpawner = NULL;
    pTopLeftSwingSpawner = NULL;
    pLeftStrafeSpawner = NULL;
    pRightStrafeSpawner = NULL;

    GameTimer = 0.0f;
    GameSteps = 0;

    playersStartLocation = FVector(0, 0, 0);
    PlayerPoints = 0;
    PlayerHighScore = 0;

}

void ASpaaaceGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (pTopRightSwingSpawner == NULL)
    {
        UClass* ClassToFind = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Spawner_BP.Spawner_BP_C'"));

        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

        for (int i = 0; i < FoundActors.Num(); i++)
        {
            if (pTopRightSwingSpawner == NULL && FoundActors[i]->GetFName().ToString().Compare("TopRightSwing") == 0)
            {
                pTopRightSwingSpawner = Cast<ASpawner>(FoundActors[i]);
            }
            else if (pTopLeftSwingSpawner == NULL && FoundActors[i]->GetFName().ToString().Compare("TopLeftSwing") == 0)
            {
                pTopLeftSwingSpawner = Cast<ASpawner>(FoundActors[i]);
            }
            else if (pLeftStrafeSpawner == NULL && FoundActors[i]->GetFName().ToString().Compare("LeftStrafe") == 0)
            {
                pLeftStrafeSpawner = Cast<ASpawner>(FoundActors[i]);
            }
            else if (pRightStrafeSpawner == NULL && FoundActors[i]->GetFName().ToString().Compare("RightStrafe") == 0)
            {
                pRightStrafeSpawner = Cast<ASpawner>(FoundActors[i]);
            }
        }
    }

    if (pPlayerShip == NULL)
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShip::StaticClass(), FoundActors);

        for (int i = 0; i < FoundActors.Num(); i++)
        {
            if (!FoundActors[i]->GetClass()->IsChildOf<AAIShip>())
            {
                pPlayerShip = FoundActors[i];
                playersStartLocation = pPlayerShip->GetActorLocation();
            }
        }
    }
    if (pAIShipClass == NULL)
    {
        pAIShipClass = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/AIShip_BP.AIShip_BP_C'"));
    }

    if (GameSteps == 0 && pTopRightSwingSpawner != NULL && pPlayerShip != NULL && pAIShipClass != NULL)
    {
        GameSteps = 1;
        pTopRightSwingSpawner->SetPlayerShip(pPlayerShip);
        pTopRightSwingSpawner->SetShipType(pAIShipClass);

        pTopLeftSwingSpawner->SetPlayerShip(pPlayerShip);
        pTopLeftSwingSpawner->SetShipType(pAIShipClass);

        pLeftStrafeSpawner->SetPlayerShip(pPlayerShip);
        pLeftStrafeSpawner->SetShipType(pAIShipClass);

        pRightStrafeSpawner->SetPlayerShip(pPlayerShip);
        pRightStrafeSpawner->SetShipType(pAIShipClass);
    }
    else
    {
        HandleGameSteps(DeltaTime);
    }
}

void ASpaaaceGameMode::HandleGameSteps(float DeltaTime)
{
    GameTimer += DeltaTime;

    switch (GameSteps)
    {
    case 1:
        if (GameTimer > 10)
        {
            CLIENTCOMPLAINT("STARTING WAVE 1");
            pTopRightSwingSpawner->SpawnWave(5);
            GameSteps = 2;
        }
        break;
    case 2:
        if (GameTimer > 20)
        {
            CLIENTCOMPLAINT("STARTING WAVE 2");
            pTopLeftSwingSpawner->SpawnWave(5);
            GameSteps = 3;
        }
        break;
    case 3:
        if (GameTimer > 30)
        {
            CLIENTCOMPLAINT("STARTING WAVE 3");
            pLeftStrafeSpawner->SpawnWave(5);
            pRightStrafeSpawner->SpawnWave(5);
            GameSteps = 4;
        }
        break;

    default:
        break;
    }
}

void ASpaaaceGameMode::PlayerWasHit()
{
    pTopRightSwingSpawner->StopSpawning();
    GameTimer = 0;
    GameSteps = 1;
    PlayerHighScore = FMath::Max(PlayerHighScore, PlayerPoints);
    PlayerPoints = 0;

    UClass* BulletClass = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Bullet_BP.Bullet_BP_C'"));

    TArray<AActor*> FoundBullets;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), BulletClass, FoundBullets);

    for (int i = 0; i < FoundBullets.Num(); i++)
    {
        FoundBullets[i]->Destroy();
    }

    UClass* ASIShipClass = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/AIShip_BP.AIShip_BP_C'"));

    TArray<AActor*> FoundShips;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASIShipClass, FoundShips);

    for (int i = 0; i < FoundShips.Num(); i++)
    {
        FoundShips[i]->Destroy();
    }

    pPlayerShip->SetActorLocation(playersStartLocation);
}

void ASpaaaceGameMode::PlayerKilledMe(int numPoints, FVector location)
{
    CLIENTCOMPLAINT2("PLAYER SCORE: %d", PlayerPoints);
}
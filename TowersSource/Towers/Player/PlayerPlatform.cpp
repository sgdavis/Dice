// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPlatform.h"
#include "Game/TowersGameMode.h"
#include "Game/TowerGameStateBase.h"
#include "Game/Goal.h"
#include "Tower/Tower.h"
#include "Minion/Minion.h"

// Sets default values
APlayerPlatform::APlayerPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bPlayerTeleported = false;
	pMyPlayer = NULL;
	pPlayerSpawn = NULL;
	pPadSpawn = NULL;
	bHasRegisteredFireRateCallback = false;
}

// Called when the game starts or when spawned
void APlayerPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pPlayerSpawn == NULL || pEnemySpawn == NULL || pPadSpawn == NULL || pTowerSpawn == NULL)
	{
		TSet<UActorComponent*> components = GetComponents();
		SERVERCOMPLAINT2(TEXT("PLATFORM HAS %d COMPONENTS"), components.Num());
		for (UActorComponent* component : components)
		{
			SERVERCOMPLAINT2(TEXT("COMPONENT NAME %s"), *component->GetFName().ToString());
			if (pPlayerSpawn == NULL && component->GetFName().IsEqual("PlayerSpawn"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE PLAYER SPAWN"));
				pPlayerSpawn = Cast<UStaticMeshComponent>(component);
			}
			else if (pEnemySpawn == NULL && component->GetFName().IsEqual("EnemySpawn"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE ENEMY SPAWN"));
				pEnemySpawn = Cast<UStaticMeshComponent>(component);
			}
			else if (pPadSpawn == NULL && component->GetFName().IsEqual("PadSpawn"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE SPAWN SOAWN"));
				pPadSpawn = Cast<UStaticMeshComponent>(component);
				ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();

				if (GM != NULL)
				{
					pSpawnPad = GM->PlaceSpawnPoint(pMyPlayer, pPadSpawn->GetComponentLocation(), pPadSpawn->GetComponentRotation());
					pSpawnPad->SetPlatform(this);
				}
			}
			else if (pGoal == NULL && component->GetFName().IsEqual("GoalSpawn"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE GOAL SPAWN"));
				pGoalSpawn = Cast<UStaticMeshComponent>(component);
				ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();

				if (GM != NULL)
				{
					pGoal = GM->PlaceGoal(pGoalSpawn->GetComponentLocation(), pGoalSpawn->GetComponentRotation());
					pGoal->SetPlatform(this);
				}
			}
			else if (pTower == NULL && component->GetFName().IsEqual("TowerSpawn"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE TOWER SPAWN"));
				pTowerSpawn = Cast<UStaticMeshComponent>(component);
				ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();

				if (GM != NULL)
				{
					pTower = GM->PlaceTower(pTowerSpawn->GetComponentLocation(), pTowerSpawn->GetComponentRotation());
					pTower->SetPlatform(this);
				}
			}
		}
	}

	if (!bPlayerTeleported && pMyPlayer && pMyPlayer->IsReadyToTeleport() && pPlayerSpawn)
	{
		SERVERCOMPLAINT(TEXT("SHOULD TELEPORT"));
		bPlayerTeleported = true;
		pMyPlayer->TeleportPlayer(pPlayerSpawn->GetComponentLocation());
	}
}

void APlayerPlatform::SetPlatformPlayer(ATowersPlayerController* player)
{
	pMyPlayer = Cast<ATowersPlayerController>(player);
}

ATowersPlayerController* APlayerPlatform::GetPlatformPlayer()
{
	return pMyPlayer;
}

ASpawnPad* APlayerPlatform::GetSpawner()
{
	return pSpawnPad;
}

AGoal* APlayerPlatform::GetGoal()
{
	return pGoal;
}

ATower* APlayerPlatform::GetTower()
{
	return pTower;
}

FVector APlayerPlatform::GetEnemyLocation()
{
	if (pEnemySpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] had location"));
		return pEnemySpawn->GetComponentLocation();
	}
	else
	{
		SERVERCOMPLAINT(TEXT("[SERVER] oops"));
		return FVector(0,0,0);
	}
}

FRotator APlayerPlatform::GetEnemyRotator()
{
	if (pEnemySpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] had rotation"));
		return pEnemySpawn->GetComponentRotation();
	}
	else
	{
		SERVERCOMPLAINT(TEXT("[SERVER] oops"));
		return FRotator(0,0,0);
	}
}

bool APlayerPlatform::OnDamageTaken_Validate(int amount)
{
	return true;
}

void APlayerPlatform::OnDamageTaken_Implementation(int amount)
{
	SERVERCOMPLAINT(TEXT("A client has been hit"));
	ATowersGameMode * GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();

	if (GM != NULL)
	{
		GM->NotifyHPChange(pMyPlayer->GetPlayerIndex(), amount*-1);
	}
}

void APlayerPlatform::AddMinion(AMinion* minion)
{
	SERVERCOMPLAINT(TEXT("ADDING A MINION"));
	minions.Add(minion);
	minion->SetPlatform(this);
	SERVERCOMPLAINT2(TEXT("WE NOW HAVE %d MINIONS"), minions.Num());
}

void APlayerPlatform::RemoveMinion(AMinion* minion, bool killed)
{
	minions.Remove(minion);
	if (pTower->GetTarget() == minion)
	{
		pTower->ClearTarget();
	}
	if (killed && pMyPlayer != NULL)
	{
		pMyPlayer->RequestReward(minion->GetReward());
	}
}

AMinion* APlayerPlatform::GetClosestMinion()
{
	AMinion* closestMinion = NULL;
	float closestDistance = -1;

	CleanMinions();

	for (AMinion* minion : minions)
	{
		int distance = FVector::DistSquared(minion->GetActorLocation(), pGoal->GetActorLocation());

		if (closestDistance == -1 || distance < closestDistance)
		{
			closestDistance = distance;
			closestMinion = minion;
		}
	}

	return closestMinion;
}

void APlayerPlatform::CleanMinions()
{
	for (int i = minions.Num()-1; i >- 0; i--)
	{
		if (minions[i] == NULL || minions[i]->IsPendingKill())
		{
			minions.RemoveAt(i);
		}
	}
}
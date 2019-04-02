// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TowersGameMode.h"
#include "Player/TowersPlayerController.h"
#include "Player/TowersCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Minion/Minion.h"
#include "Minion/SpawnPad.h"
#include "Game/Goal.h"
#include "Tower/Tower.h"
#include "Tower/Projectile.h"
#include "Game/TowerGameStateBase.h"

ATowersGameMode::ATowersGameMode()
{
	GameStateClass = ATowerGameState::StaticClass();

	// use our custom PlayerController class
	PlayerControllerClass = ATowersPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	player1StartPoint = NULL;
	player2StartPoint = NULL;

	for (AActor* TActor : FoundActors)
	{
		if (TActor != nullptr)
		{
			if (player1StartPoint == NULL)
			{
				player1StartPoint = Cast<APlayerStart>(TActor);
			}
			else if (player2StartPoint == NULL)
			{
				player2StartPoint = Cast<APlayerStart>(TActor);
			}
		}
	}

	player1Spawned = false;
	player2Spawned = false;

	numPlayers = 0;

	payoutTimer = PAYOUTDELAY;
}

// Called every frame
void ATowersGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
	{
		if (--payoutTimer == 0)
		{
			payoutTimer = PAYOUTDELAY;
			GS->NotifyPayouts();
		}
	}
}

AActor* ATowersGameMode::ChoosePlayerStart_Implementation(AController* player)
{
	PlacePlayerPlatform(Cast< ATowersPlayerController>(player), numPlayers++);

	if (player1Spawned == false)
	{
		player1Spawned = true;
		return player1StartPoint;
	}
	else if (player2Spawned == false)
	{
		player2Spawned = true;
		return player2StartPoint;
	}

	return player1StartPoint;
}

void ATowersGameMode::RequestFireProjectile(ATower* tower)
{
	SERVERCOMPLAINT(TEXT("[SERVER] requested"));
	UClass* projectileSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Projectile_BP.Projectile_BP_C'"));
	if (projectileSpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] has projectile"));
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			SERVERCOMPLAINT(TEXT("[SERVER] has world"));
			FActorSpawnParameters parameters;
			parameters.Owner = this;

			AProjectile* newProjectile = pWorld->SpawnActor<AProjectile>(projectileSpawn, tower->GetFiringPoint()->GetComponentLocation(), tower->GetFiringPoint()->GetComponentRotation());
			if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
			{
				TArray<FPlayerStats> playerList = GS->GetPlayerList();
				int damage = 1;
				int speed = 1;
				for (int i = 0; i < playerList.Num(); i++)
				{
					if (tower &&
						tower->GetPlatform() &&
						tower->GetPlatform()->GetPlatformPlayer() &&
						tower->GetPlatform()->GetPlatformPlayer()->GetPlayerIndex() == playerList[i].playerIndex)
					{
						damage = damage + playerList[i].upgrades[STATUPGRADE::TowerDamage];
						speed = speed + playerList[i].upgrades[STATUPGRADE::TowerShotSpeed];
						break;
					}
				}

				newProjectile->SetStats(damage, speed);
			}

			SERVERCOMPLAINT(TEXT("[SERVER] should be spawned"));
		}
	}
}

ASpawnPad* ATowersGameMode::PlaceSpawnPoint(AController* player, FVector spawnLocation, FRotator rotator)
{
	ASpawnPad* pSpawnPad = NULL;
	SERVERCOMPLAINT(TEXT("[SERVER] placing spawn"));
	UClass* spawnerSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/SpawnPad_BP.SpawnPad_BP_C'"));
	if (spawnerSpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] has spawner"));
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			SERVERCOMPLAINT(TEXT("[SERVER] has world"));
			FActorSpawnParameters parameters;
			parameters.Owner = this;

			pSpawnPad = pWorld->SpawnActor<ASpawnPad>(spawnerSpawn, spawnLocation, rotator, parameters);
			if (pSpawnPad)
			{
				SERVERCOMPLAINT(TEXT("[SERVER] spawn exists"));
				pSpawnPad->SetValidPlayer(player);
			}
		}
	}
	return pSpawnPad;
}

AGoal* ATowersGameMode::PlaceGoal(FVector spawnLocation, FRotator rotator)
{
	AGoal* pGoal = NULL;
	SERVERCOMPLAINT(TEXT("[SERVER] placing spawn"));
	UClass* goalSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Goal_BP.Goal_BP_C'"));
	if (goalSpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] has spawner"));
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			SERVERCOMPLAINT(TEXT("[SERVER] has world"));
			FActorSpawnParameters parameters;
			parameters.Owner = this;

			pGoal = pWorld->SpawnActor<AGoal>(goalSpawn, spawnLocation, rotator, parameters);
			if (pGoal)
			{
				SERVERCOMPLAINT(TEXT("[SERVER] goal exists"));
			}
		}
	}
	return pGoal;
}

ATower* ATowersGameMode::PlaceTower(FVector spawnLocation, FRotator rotator)
{
	ATower* pTower = NULL;
	SERVERCOMPLAINT(TEXT("[SERVER] placing spawn"));
	UClass* towerSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Tower_BP.Tower_BP_C'"));
	if (towerSpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] has spawner"));
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			SERVERCOMPLAINT(TEXT("[SERVER] has world"));
			FActorSpawnParameters parameters;
			parameters.Owner = this;

			pTower = pWorld->SpawnActor<ATower>(towerSpawn, spawnLocation, rotator, parameters);
			if (pTower)
			{
				SERVERCOMPLAINT(TEXT("[SERVER] goal exists"));
			}
		}
	}
	return pTower;
}

void ATowersGameMode::PlacePlayerPlatform(ATowersPlayerController* player, int playerNumber)
{
	SERVERCOMPLAINT(TEXT("[SERVER] placing platform"));
	UClass* spawnerSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/PlayerPlatform_BP.PlayerPlatform_BP_C'"));
	if (spawnerSpawn)
	{
		SERVERCOMPLAINT(TEXT("[SERVER] has platform"));
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			SERVERCOMPLAINT(TEXT("[SERVER] has world"));
			FActorSpawnParameters parameters;
			parameters.Owner = this;

			FRotator rotator;
			rotator.Pitch = 0.0f;
			rotator.Roll = 0.0f;
			rotator.Yaw = 0.0f;

			FVector spawnLocation;
			spawnLocation.X = -270.0f;
			spawnLocation.Y = playerNumber * 1000.0f;
			spawnLocation.Z = 500.0f;

			APlayerPlatform* pPlatform = pWorld->SpawnActor<APlayerPlatform>(spawnerSpawn, spawnLocation, rotator, parameters);
			if (pPlatform)
			{
				SERVERCOMPLAINT2(TEXT("[SERVER] platform exists %d"), pPlatform->GetComponents().Num());
				pPlatform->SetPlatformPlayer(player);
				platforms.Add(pPlatform);

				if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
				{
					SERVERCOMPLAINT(TEXT("[SERVER] game state exists"));
					ATowersPlayerController* playerController = Cast< ATowersPlayerController>(player);
					playerController->SetPlayerIndex(numPlayers);
					GS->AddPlayer(numPlayers, pPlatform);
				}
			}
		}
	}
}

void ATowersGameMode::NotifyHPChange(int playerIndex, int amount)
{
	SERVERCOMPLAINT(TEXT("damage has been taken"));

	if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
	{
		APlayerPlatform* platform = GS->NotifyHPChange(playerIndex, amount);
		if (platform)
		{
			platform->GetSpawner()->Destroy();
			platform->GetGoal()->Destroy();
			platform->Destroy();
		}

		if (GS->GetNumPlayersAlive() <= 1)
		{
			RestartGame();
		}
	}
}

void ATowersGameMode::RequestSpawn(int playerIndex)
{
	UClass* minionSpawn = LoadObject<UClass>(this, *FString("Class'/Game/TopDownCPP/Blueprints/Minion_BP.Minion_BP_C'"));
	if (minionSpawn)
	{
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			FActorSpawnParameters parameters;
			parameters.Owner = this;

			if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
			{
				TArray<FPlayerStats> playerList = GS->GetPlayerList();
				int damage = 1;
				int speed = 1;
				int hp = 1;
				int reward = 1;
				for (int i = 0; i < playerList.Num(); i++)
				{
					if (playerIndex == playerList[i].playerIndex)
					{
						if (playerList[i].cash < playerList[i].spawnCost)
						{
							return;
						}

						GS->ModifyIncome(playerIndex, 1);
						GS->ChargePlayer(playerIndex, playerList[i].spawnCost);
						damage = damage + playerList[i].upgrades[STATUPGRADE::MinionDamage];
						speed = speed + playerList[i].upgrades[STATUPGRADE::MinionSpeed];
						hp = hp + playerList[i].upgrades[STATUPGRADE::MinionHitPoints];
						reward = FMath::CeilToInt(playerList[i].spawnCost / 2.0f);
						break;
					}
				}

				for (int i = 0; i < playerList.Num(); i++)
				{
					if (playerIndex == playerList[i].playerIndex)
					{
						continue;
					}
					APlayerPlatform* spawningPlatform = playerList[i].platform;

					for (int j = 0; j < 20; j++)
					{
						FVector spawningLocation = spawningPlatform->GetEnemyLocation();
						spawningLocation.Y = spawningLocation.Y + (FMath::Rand() % 400 - 200);

						AMinion* newMinion = pWorld->SpawnActor<AMinion>(minionSpawn, spawningLocation, spawningPlatform->GetEnemyRotator(), parameters);

						if (newMinion != NULL)
						{
							int modifiedSpeed = FMath::Max(1, speed - playerList[i].upgrades[STATUPGRADE::LaneSlow]);
							newMinion->SetStats(damage, modifiedSpeed, hp, reward);
							spawningPlatform->AddMinion(newMinion);
							break;
						}
					}
				}
			}
		}
	}
}

void ATowersGameMode::RequestUpgrade(int playerIndex, int upgradeIndex)
{
	if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
	{
		TArray<FPlayerStats> playerList = GS->GetPlayerList();

		for (int i = 0; i < playerList.Num(); i++)
		{
			if (playerIndex == playerList[i].playerIndex)
			{
				if (playerList[i].cash < playerList[i].costs[upgradeIndex])
				{
					break;
				}

				GS->ChargePlayer(playerIndex, playerList[i].costs[upgradeIndex]);
				GS->NotifyUpgrade(playerIndex, upgradeIndex, 1, 1.5f);

				if (upgradeIndex == STATUPGRADE::TowerFiringSpeed)
				{
					playerList[i].platform->GetTower()->SetFiringDelay(playerList[i].upgrades[STATUPGRADE::TowerFiringSpeed] + 1);
				}
				else if (upgradeIndex >= STATUPGRADE::MinionHitPoints && upgradeIndex <= STATUPGRADE::MinionSpeed)
				{
					GS->ModifySpawnCost(playerIndex, FMath::CeilToInt(playerList[i].costs[upgradeIndex] / 2.0f));
				}

				break;
			}
		}
	}
}

void ATowersGameMode::RequestReward(int playerIndex, int reward)
{
	if (ATowerGameState* GS = GetWorld()->GetGameState<ATowerGameState>())
	{
		TArray<FPlayerStats> playerList = GS->GetPlayerList();

		for (int i = 0; i < playerList.Num(); i++)
		{
			if (playerIndex == playerList[i].playerIndex)
			{
				GS->ChargePlayer(playerIndex, reward * -1);

				break;
			}
		}
	}
}
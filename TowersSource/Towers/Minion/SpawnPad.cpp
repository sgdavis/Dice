// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPad.h"
#include "Player/PlayerPlatform.h"
#include "Game/TowersGameMode.h"

// Sets default values
ASpawnPad::ASpawnPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pMyPlatform = NULL;
}

// Called when the game starts or when spawned
void ASpawnPad::BeginPlay()
{
	Super::BeginPlay();
	
	nCountdown = SPAWNCOUNTER;

	TArray<UStaticMeshComponent*> allMeshes;
	GetComponents<UStaticMeshComponent>(allMeshes);

	for (int32 i = 0; i < allMeshes.Num(); i++)
	{
		if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("Light25a"))
		{
			CLIENTCOMPLAINT(TEXT("got 25"));
			Light25 = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("Light50a"))
		{
			CLIENTCOMPLAINT(TEXT("got 50"));
			Light50 = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("Light75a"))
		{
			CLIENTCOMPLAINT(TEXT("got 75"));
			Light75 = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("Light100a"))
		{
			CLIENTCOMPLAINT(TEXT("got 100"));
			Light100 = allMeshes[i];
		}
	}

	Light25->SetVisibility(false);
	Light50->SetVisibility(false);
	Light75->SetVisibility(false);
	Light100->SetVisibility(false);
}

// Called every frame
void ASpawnPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pMyPlayer == NULL)
	{
		return;
	}

	if (bCountingDown)
	{
		float percentage = 1 - ((float)nCountdown / SPAWNCOUNTER);
		CLIENTCOMPLAINT2(TEXT("ticking %f"), percentage);
		if (--nCountdown == 0)
		{
			nCountdown = SPAWNCOUNTER;

			SetLightVisible(25, false);
			SetLightVisible(50, false);
			SetLightVisible(75, false);
			SetLightVisible(100, false);
		}
		else if (percentage > 0.9f)
		{
			SetLightVisible(100, true);
		}
		else if (percentage > 0.75f)
		{
			SetLightVisible(75, true);
		}
		else if (percentage > 0.50f)
		{
			SetLightVisible(50, true);
		}
		else if (percentage > 0.25f)
		{
			SetLightVisible(25, true);
		}
	}
}

void ASpawnPad::StartCountdown(AActor* otherActor)
{
	if (pMyPlayer == NULL || pMyPlayer->GetPawn() != otherActor)
	{
		return;
	}

	bCountingDown = true;
	nCountdown = SPAWNCOUNTER;
}

void ASpawnPad::StopCountdown(AActor* otherActor)
{
	if (pMyPlayer == NULL || pMyPlayer->GetPawn() != otherActor)
	{
		return;
	}

	bCountingDown = false;

	SetLightVisible(25, false);
	SetLightVisible(50, false);
	SetLightVisible(75, false);
	SetLightVisible(100, false);
}

void ASpawnPad::SetValidPlayer(AController* player)
{
	pMyPlayer = player;
}

bool ASpawnPad::SetLightVisible_Validate(int number, bool visible)
{
	return true;
}

void ASpawnPad::SetLightVisible_Implementation(int number, bool visible)
{
	if (number == 25)
	{
		Light25->SetVisibility(visible);
	}
	else if (number == 50)
	{
		Light50->SetVisibility(visible);
	}
	else if (number == 75)
	{
		Light75->SetVisibility(visible);
	}
	else if (number == 100)
	{
		Light100->SetVisibility(visible);
	}
}

void ASpawnPad::SetPlatform(APlayerPlatform* platform)
{
	pMyPlatform = platform;
}

APlayerPlatform* ASpawnPad::GetPlatform()
{
	return pMyPlatform;
}
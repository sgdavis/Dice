// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal.h"
#include "Game/TowersGameMode.h"
#include "Minion/Minion.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pPlatform = NULL;
	nHitPoints = 5;
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> allMeshes;
	GetComponents<UStaticMeshComponent>(allMeshes);

	for (int32 i = 0; i < allMeshes.Num(); i++)
	{
		if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("HP1"))
		{
			HP1Sphere = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("HP2"))
		{
			HP2Sphere = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("HP3"))
		{
			HP3Sphere = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("HP4"))
		{
			HP4Sphere = allMeshes[i];
		}
		else if (allMeshes[i] && allMeshes[i]->GetFName().IsEqual("HP5"))
		{
			HP5Sphere = allMeshes[i];
		}
	}
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::OnDamageTaken(AActor* otherActor)
{
	if (otherActor == NULL || otherActor->GetClass() == NULL || !otherActor->GetClass()->IsChildOf<AMinion>())
	{
		return;
	}

	AMinion* pMinion = Cast<AMinion>(otherActor);

	if (nHitPoints == 5)
	{
		HP5Sphere->SetVisibility(false);
	}
	else if (nHitPoints == 4)
	{
		HP4Sphere->SetVisibility(false);
	}
	else if (nHitPoints == 3)
	{
		HP3Sphere->SetVisibility(false);
	}
	else if (nHitPoints == 2)
	{
		HP2Sphere->SetVisibility(false);
	}
	else if (nHitPoints == 1)
	{
		HP1Sphere->SetVisibility(false);
	}
	nHitPoints = nHitPoints - 1;

	CLIENTCOMPLAINT(TEXT("I've been hit"));
	if (pPlatform)
	{
		pPlatform->OnDamageTaken(pMinion->GetDamage());
	}
}

void AGoal::SetPlatform(APlayerPlatform* platform)
{
	pPlatform = platform;
}


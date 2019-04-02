// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Minion/Minion.h"
#include "Game/TowersGameMode.h"

// Sets default values
ATower::ATower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pPlatform = NULL;
	pCurrentTarget = NULL;
	firingDelay = NORMALFIRINGDELAY;
	firingDelayCounter = 0;
}

void ATower::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATower, firingDelay);
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	if (pSwivelPoint == NULL)
	{
		TSet<UActorComponent*> components = GetComponents();
		SERVERCOMPLAINT2(TEXT("PLATFORM HAS %d COMPONENTS"), components.Num());
		for (UActorComponent* component : components)
		{
			SERVERCOMPLAINT2(TEXT("COMPONENT NAME %s"), *component->GetFName().ToString());
			if (pSwivelPoint == NULL && component->GetFName().IsEqual("Pivot"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE PIVOT"));
				pSwivelPoint = Cast<UStaticMeshComponent>(component);
			}
			if (pFiringPoint == NULL && component->GetFName().IsEqual("SpawnPoint"))
			{
				SERVERCOMPLAINT(TEXT("FOUND THE FIRING POINT"));
				pFiringPoint = Cast<UStaticMeshComponent>(component);
			}
		}
	}
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pPlatform != NULL)
	{
		if (pCurrentTarget == NULL || pCurrentTarget->IsPendingKill())
		{
			pCurrentTarget = pPlatform->GetClosestMinion();
			firingDelayCounter = firingDelay;
		}

		if (pCurrentTarget != NULL && !pCurrentTarget->IsPendingKill())
		{
			TrackTarget(pCurrentTarget->GetActorLocation());
			firingDelayCounter = firingDelayCounter - 1;
			if (firingDelayCounter == 0)
			{
				FireProjectile();
				firingDelayCounter = firingDelay;
			}
		}
	}
}

void ATower::SetPlatform(APlayerPlatform* platform)
{
	pPlatform = platform;
}

APlayerPlatform* ATower::GetPlatform()
{
	return pPlatform;
}

bool ATower::TrackTarget_Validate(FVector targetLocation)
{
	return true;
}

void ATower::TrackTarget_Implementation(FVector targetLocation)
{
	if (pSwivelPoint != NULL)
	{
		FRotator newrot = (targetLocation - pSwivelPoint->GetComponentLocation()).Rotation();
		pSwivelPoint->SetWorldRotation(newrot);
	}
}

void ATower::FireProjectile()
{
	CLIENTCOMPLAINT(TEXT("REQUESTING SPAWN"));
	ServerFireProjectile();
}

bool ATower::ServerFireProjectile_Validate()
{
	SERVERCOMPLAINT(TEXT("[SERVER] validate"));
	return true;
}

void ATower::ServerFireProjectile_Implementation()
{
	SERVERCOMPLAINT(TEXT("[SERVER] implementation"));
	ATowersGameMode* GM = GetWorld()->GetAuthGameMode<ATowersGameMode>();
	if (GM)
	{
		GM->RequestFireProjectile(this);
	}
}

UStaticMeshComponent* ATower::GetFiringPoint()
{
	return pFiringPoint;
}

void ATower::SetFiringDelay(int delay)
{
	CLIENTCOMPLAINT2(TEXT("SETTING NEW FIRING DELAY %d"), NORMALFIRINGDELAY - (5 * delay));
	firingDelay = NORMALFIRINGDELAY - (5 * delay);
}
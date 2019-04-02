// Fill out your copyright notice in the Description page of Project Settings.

#include "Minion.h"
#include "Game/Goal.h"

// Sets default values
AMinion::AMinion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	damageValue = 1;
	movementSpeed = 0.04;
	hitPoints = 1;
	rewardGold = 0;
}

void AMinion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMinion, damageValue);
	DOREPLIFETIME(AMinion, movementSpeed);
	DOREPLIFETIME(AMinion, hitPoints);
	DOREPLIFETIME(AMinion, rewardGold);
}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector movement = GetActorForwardVector();
	movement.Normalize();

	AddMovementInput(movement, movementSpeed);
}

void AMinion::OnDamageDelivered(AActor* otherActor)
{
	if (otherActor == NULL || otherActor->GetClass() == NULL || !otherActor->GetClass()->IsChildOf<AGoal>())
	{
		return;
	}

	if (pMyPlatform != NULL)
	{
		pMyPlatform->RemoveMinion(this, false);
	}
	Destroy();
}

void AMinion::OnDamageTaken(int amount)
{
	hitPoints = hitPoints - amount;
	if (hitPoints <= 0)
	{
		if (pMyPlatform != NULL)
		{
			pMyPlatform->RemoveMinion(this, true);
		}
		Destroy();
	}
}

void AMinion::SetStats(int damage, int speed, int hp, int reward)
{
	damageValue = damage; 
	movementSpeed = speed * movementSpeed;
	hitPoints = hp;
	rewardGold = reward;
}
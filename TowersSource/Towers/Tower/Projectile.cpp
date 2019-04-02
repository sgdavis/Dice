// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Minion/Minion.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	damageValue = 1;
	movementSpeed = 10;
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectile, damageValue);
	DOREPLIFETIME(AProjectile, movementSpeed);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector movement = GetActorForwardVector();
	movement.Normalize();
	movement *= movementSpeed;

	FVector position = GetActorLocation();

	SetActorLocation(position+movement);
}

void AProjectile::OnImpact(AActor* otherActor)
{
	if (otherActor == NULL || otherActor->GetClass() == NULL || !otherActor->GetClass()->IsChildOf<AMinion>())
	{
		return;
	}

	AMinion* minion = Cast<AMinion>(otherActor);
	minion->OnDamageTaken(damageValue);

	Destroy();
}

void AProjectile::SetStats(int damage, int speed)
{
	damageValue = damage;
	movementSpeed = movementSpeed * (1 + speed/3.0f);
}
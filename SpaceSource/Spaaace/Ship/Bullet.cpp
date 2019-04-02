// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Ship.h"
#include "AIShip.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    speed = 300.0f;
    damage = 1;
    lifetime = 9.0f;
    bIsAIFired = false;

    SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    /*FVector newLocation = GetActorLocation() + DeltaTime * speed * GetActorForwardVector();
    SetActorLocation(newLocation, true);*/

    lifetime -= DeltaTime;
    if (lifetime < 0.0f)
    {
        Destroy();
    }
}

void ABullet::HitSomething(AActor* otherActor, UActorComponent* otherComponent)
{
    if (otherActor == NULL || otherActor->GetClass() == NULL || !otherActor->GetClass()->IsChildOf<AShip>())
    {
        return;
    }

    if (bIsAIFired && otherActor->GetClass()->IsChildOf<AAIShip>())
    {
        return;
    }
    else if(!bIsAIFired && !otherActor->GetClass()->IsChildOf<AAIShip>())
    {
        return;
    }

    CLIENTCOMPLAINT2("HIT THIS THING %s", *otherActor->GetName());
    CLIENTCOMPLAINT2("IN THIS THING %s", *otherComponent->GetName());

    AShip* pShip = Cast<AShip>(otherActor);
    pShip->WasHit(damage);
    Destroy(true);
}
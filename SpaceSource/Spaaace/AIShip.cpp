// Fill out your copyright notice in the Description page of Project Settings.

#include "AIShip.h"
#include "SpaaaceGameMode.h"

AAIShip::AAIShip() : AShip()
{
    firing = false;
    firingTimer = 0;
    firingDelay = 50;
    bIsAIShip = true;

    percentage = 0.0f;
    pTarget = NULL;

    hitPoints = 0;
    scorePoints = 1;
}

// Called every frame
void AAIShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (firingTimer-- <= 0)
    {
        FireWeapon();
        firingTimer = firingDelay;
    }

    percentage = percentage + 0.001f;
    if (percentage > 1.0f)
    {
        Destroy();
    }
    else
    {
        float distance = percentage * pSplineToFollow->GetSplineLength();
        FVector newPosition = pSplineToFollow->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
        FRotator newRotation = pSplineToFollow->GetRotationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);

        SetActorLocationAndRotation(newPosition, newRotation);
    }
}

FRotator AAIShip::GetWeaponDirection()
{
    FVector weaponLocation = GetWeaponLocation();
    FVector targetLocation = FVector(0,0,0);
    if (pTarget != NULL)
    {
        targetLocation = pTarget->GetActorLocation();
    }
    
    return (targetLocation - weaponLocation).Rotation();
}

void AAIShip::WasHit(int damage)
{
    hitPoints -= damage;

    if (hitPoints <= 0)
    {
        ASpaaaceGameMode* GM = GetWorld()->GetAuthGameMode<ASpaaaceGameMode>();
        if (GM)
        {
            GM->PlayerKilledMe(scorePoints, GetActorLocation());
        }
        Destroy();
    }
}
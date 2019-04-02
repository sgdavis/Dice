// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship/Ship.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "AIShip.generated.h"

/**
 * 
 */
UCLASS()
class SPAAACE_API AAIShip : public AShip
{
    GENERATED_BODY()

    AAIShip();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
    void SetFollowSpline(USplineComponent* toFollow) { pSplineToFollow = toFollow; }
    USplineComponent* pSplineToFollow;

    float percentage;

    int hitPoints;
    int scorePoints;

    AActor* pTarget;
    void SetTarget(AActor* target) { pTarget = target; }

    virtual FRotator GetWeaponDirection();
    void WasHit(int damage);
};

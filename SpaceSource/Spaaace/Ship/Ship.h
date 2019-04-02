// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Ship.generated.h"

#define BANK_ROTATION 20

UCLASS()
class SPAAACE_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void SelectWeapon();
    virtual FVector GetWeaponLocation();
    virtual FRotator GetWeaponDirection();

    float Roll;
    float GetRoll() { return Roll; }
    float GetShipSpeed(float axisValue);
    float GetBankSpeed(float axisValue);
    void HandleBankAngle(float axisValue);
    void StartFiringWeapon() { firing = true; }
    void StopFiringWeapon() { firing = false; }

    void MovingForwardBack(float axisValue);
    void MovingLeftRight(float axisValue);
    void FireWeapon();

    virtual void WasHit(int damage);
    void SpawnBullet();

    UStaticMeshComponent* pBody;
    FRotator BodyRotation;

    int speed;
    int firingDelay;
    int firingTimer;
    bool firing;
    TArray<UStaticMeshComponent*> FiringPoints;
    int currentFiringPoint;

    bool bIsAIShip;
};

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Complain.h"
#include "GameFramework/Actor.h"
#include "RPGEquipment.generated.h"

UCLASS()
class ARPGEquipment: public AActor
{
	GENERATED_BODY()

public:
    ARPGEquipment();

    virtual void PullBack(int strength, int intelligence, int dexterity);
    virtual void Swing(int strength, int intelligence, int dexterity);
    virtual void Crouch(int strength, int intelligence, int dexterity);
    virtual void Jump(int strength, int intelligence, int dexterity);
    virtual void RaiseShield(int strength, int intelligence, int dexterity);
    virtual void LowerShield(int strength, int intelligence, int dexterity);
    UFUNCTION(BlueprintCallable, category = "Equipment Functions")
    virtual void Hit(AActor* otherActor);

    virtual void Update(float DeltaTime, AActor* parent);

    //----------weapon----------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        FVector vPullBackLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        FRotator vPullBackRotation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        FVector vSwingLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        FRotator vSwingRotation;

    int nPullBackTimer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        int nPullBackSpeed;

    int nSwingTimer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        int nSwingSpeed;

    int nSheathTimer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        int nSheathSpeed;

    bool bCanPullBack;
    bool bCanSwing;

    //----------shield----------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Timers")
        FVector vBlockLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Timers")
        FRotator vBlockRotation;

    int nBlockTimer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Timers")
        int nBlockSpeed;

    int nRestTimer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Timers")
        int nRestSpeed;

    bool bCanBlock;

    float angleOffFacing;
    float distanceFromRoot;

    virtual FVector GetRestingLocation(AActor* parent);
    virtual FVector GetAnimatedLocation(AActor* parent);
    virtual FRotator GetAnimatedRotation(AActor* parent);

    virtual FVector GetWeaponLocation(AActor* parent);
    virtual FVector GetOffhandLocation(AActor* parent);

    virtual FRotator GetWeaponRotation(AActor* parent);
    virtual FRotator GetOffhandRotation(AActor* parent);

    virtual void UpdateWeaponTimers();
    virtual void UpdateOffhandTimers();

    virtual bool CanBlock();

    bool bWasBlocked;
};


// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RPGEquipment.h"
#include "Character/RPGCreature.h"
#include "Equipment/RPGShield.h"

class ARPGShield;
class ARPGCreature;

ARPGEquipment::ARPGEquipment()
{
    //-----weapon-----
    vPullBackLocation = FVector(-20, 30, 0);
    vSwingRotation = FRotator(0,0,0);
    nPullBackTimer = -1;
    nPullBackSpeed = 25;

    vSwingLocation = FVector(100, -60, 0);
    vSwingRotation = FRotator(0, 0, 0);
    nSwingTimer = -1;
    nSwingSpeed = 15;

    nSheathTimer = -1;
    nSheathSpeed = 25;

    bCanPullBack = true;
    bCanSwing = false;

    //-----Shield-----
    vBlockLocation = FVector(70, 20, 0);
    vBlockRotation = FRotator(0, 0, 0);
    nBlockTimer = -1;
    nBlockSpeed = 20;

    nRestTimer = -1;
    nRestSpeed = 10;

    bCanBlock = true;

    //-----Positioning-----
    distanceFromRoot = 0.0f;
    angleOffFacing = 0.0f;

    bWasBlocked = false;
}

// Called every frame.
void ARPGEquipment::Update(float DeltaSeconds, AActor* parent)
{
    if (parent == NULL)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("why")));
        }
        return;
    }

    if (distanceFromRoot == 0.0f)
    {
        FVector parentFlatVector = parent->GetActorLocation();
        parentFlatVector.Z = 0;
        FVector ourFlatVector = GetActorLocation();
        ourFlatVector.Z = 0;

        distanceFromRoot = FVector::Distance(ourFlatVector, parentFlatVector);
        FVector toAttachment = ourFlatVector - parentFlatVector;
        angleOffFacing = FMath::RadiansToDegrees(acosf(FVector::DotProduct(parent->GetActorForwardVector(), toAttachment)));

        FVector cross = FVector::CrossProduct(parent->GetActorForwardVector(), toAttachment);
        if (FVector::DotProduct(FVector(0,0,1), cross) < 0) { // Or > 0
            angleOffFacing = -angleOffFacing;
        }
    }

    UpdateWeaponTimers();
    UpdateOffhandTimers();

    FVector resting = GetRestingLocation(parent);
    FVector transformed = GetAnimatedLocation(parent);
    SetActorLocation(resting + transformed);

    FRotator transformedRotation = GetAnimatedRotation(parent);
    SetActorRotation(parent->GetActorRotation() + transformedRotation);
}

void ARPGEquipment::PullBack(int strength, int intelligence, int dexterity)
{
    if (nPullBackTimer == -1)
    {
        nPullBackTimer = 0;
    }
}
void ARPGEquipment::Swing(int strength, int intelligence, int dexterity)
{
    if (nSwingTimer == -1)
    {
        bWasBlocked = false;
        nSwingTimer = 0;
    }
}
void ARPGEquipment::Crouch(int strength, int intelligence, int dexterity)
{
}
void ARPGEquipment::Jump(int strength, int intelligence, int dexterity)
{
}
void ARPGEquipment::RaiseShield(int strength, int intelligence, int dexterity)
{
    if (nBlockTimer == -1)
    {
        nBlockTimer = 0;
    }
}
void ARPGEquipment::LowerShield(int strength, int intelligence, int dexterity)
{
    if (nRestTimer == -1)
    {
        nRestTimer = 0;
    }
}
void ARPGEquipment::Hit(AActor* otherActor)
{
    if (otherActor == NULL || otherActor->GetClass() == NULL)
    {
        return;
    }
    if (otherActor->GetClass()->IsChildOf<ARPGShield>() && !bWasBlocked)
    {
        ARPGShield* pTarget = Cast< ARPGShield>(otherActor);
        pTarget->BlockedSomething(this);
        CLIENTCOMPLAINT("WAS BLOCKED");
        bWasBlocked = true;
    }
    else if (otherActor->GetClass()->IsChildOf<ARPGCreature>() && !bWasBlocked)
    {
        CLIENTCOMPLAINT("HIT SOMEONE");
        ARPGCreature* pTarget = Cast< ARPGCreature>(otherActor);
        pTarget->WasHit();
        bWasBlocked = true;
    }
}

FVector ARPGEquipment::GetRestingLocation(AActor* parent)
{
    FVector forward = parent->GetActorForwardVector() * distanceFromRoot;
    FVector maybe;
    maybe.X = forward.X * FMath::Cos(angleOffFacing) - forward.Y * FMath::Sin(angleOffFacing);
    maybe.Y = forward.Y * FMath::Cos(angleOffFacing) + forward.X * FMath::Sin(angleOffFacing);
    return parent->GetActorLocation() + forward + maybe;
}

FVector ARPGEquipment::GetAnimatedLocation(AActor* parent)
{
    FVector ret = parent->GetTransform().Rotator().RotateVector(GetWeaponLocation(parent) + GetOffhandLocation(parent));

    return ret;
}

FRotator ARPGEquipment::GetAnimatedRotation(AActor* parent)
{
    FRotator ret = GetWeaponRotation(parent) + GetOffhandRotation(parent);

    return ret;
}

FVector ARPGEquipment::GetWeaponLocation(AActor* parent)
{
    FVector ret = FVector(0, 0, 0);

    if (nPullBackTimer != -1)
    {
        ret = vPullBackLocation * ((float)nPullBackTimer / (float)nPullBackSpeed);
    }
    else if (nSwingTimer != -1)
    {
        ret = vPullBackLocation * (1 - ((float)nSwingTimer / (float)nSwingSpeed)) + vSwingLocation * ((float)nSwingTimer / (float)nSwingSpeed);
    }
    else if (nSheathTimer != -1)
    {
        ret = vSwingLocation - vSwingLocation * ((float)nSheathTimer / (float)nSheathSpeed);
    }

    return ret;
}

FVector ARPGEquipment::GetOffhandLocation(AActor* parent)
{
    FVector ret = FVector(0, 0, 0);

    if (nBlockTimer != -1)
    {
        ret = vBlockLocation * ((float)nBlockTimer / (float)nBlockSpeed);
    }
    else if (nRestTimer != -1)
    {
        ret = vBlockLocation - vBlockLocation * ((float)nRestTimer / (float)nRestSpeed);
    }

    return ret;
}

FRotator ARPGEquipment::GetWeaponRotation(AActor* parent)
{
    FRotator ret = FRotator(0, 0, 0);

    if (nPullBackTimer != -1)
    {
        ret = vPullBackRotation * ((float)nPullBackTimer / (float)nPullBackSpeed);
    }
    else if (nSwingTimer != -1)
    {
        ret = vPullBackRotation * (1 - ((float)nSwingTimer / (float)nSwingSpeed)) + vSwingRotation * ((float)nSwingTimer / (float)nSwingSpeed);
    }
    else if (nSheathTimer != -1)
    {
        ret = vSwingRotation - vSwingRotation * ((float)nSheathTimer / (float)nSheathSpeed);
    }

    return ret;
}

FRotator ARPGEquipment::GetOffhandRotation(AActor* parent)
{
    FRotator ret = FRotator(0, 0, 0);

    if (nBlockTimer != -1)
    {
        ret = vBlockRotation * ((float)nBlockTimer / (float)nBlockSpeed);
    }
    else if (nRestTimer != -1)
    {
        ret = vBlockRotation - vBlockRotation * ((float)nRestTimer / (float)nRestSpeed);
    }

    return ret;
}

void ARPGEquipment::UpdateWeaponTimers()
{
    if (bCanPullBack && nPullBackTimer != -1 && nPullBackTimer < nPullBackSpeed)
    {
        nPullBackTimer++;
    }
    else if (bCanPullBack && nPullBackTimer != -1 && nSwingTimer != -1)
    {
        nPullBackTimer = -1;
        bCanSwing = true;
        bCanPullBack = false;
    }

    if (bCanSwing && nSwingTimer != -1 && nSwingTimer < nSwingSpeed)
    {
        nSwingTimer++;
    }
    else if (bCanSwing && nSwingTimer != -1)
    {
        nSwingTimer = -1;
        nSheathTimer = 0;
        bCanSwing = false;
    }

    if (nSheathTimer != -1 && nSheathTimer < nSheathSpeed)
    {
        nSheathTimer++;
    }
    else if (nSheathTimer != -1)
    {
        nSheathTimer = -1;
        bCanPullBack = true;
    }
}

bool ARPGEquipment::CanBlock()
{
    return bCanBlock;
}

void ARPGEquipment::UpdateOffhandTimers()
{
    if (CanBlock() && nBlockTimer != -1 && nBlockTimer < nBlockSpeed)
    {
        nBlockTimer++;
    }

    if (nBlockTimer != -1 && nRestTimer != -1 && nRestTimer < nRestSpeed)
    {
        nBlockTimer = -1;
    }
    if (nRestTimer != -1 && nRestTimer < nRestSpeed)
    {
        nRestTimer++;
    }
    else if (nRestTimer != -1)
    {
        nRestTimer = -1;
    }
}
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RPGPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "RPGCreature.h"
#include "Engine/World.h"

ARPGPlayerController::ARPGPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ARPGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

    InputComponent->BindAction("WeaponButton", IE_Pressed, this, &ARPGPlayerController::WeaponButtonDown);
    InputComponent->BindAction("WeaponButton", IE_Released, this, &ARPGPlayerController::WeaponButtonUp);

    InputComponent->BindAction("BlockButton", IE_Pressed, this, &ARPGPlayerController::BlockButtonDown);
    InputComponent->BindAction("BlockButton", IE_Released, this, &ARPGPlayerController::BlockButtonUp);

    InputComponent->BindAction("JumpButton", IE_Pressed, this, &ARPGPlayerController::JumpButtonDown);
    InputComponent->BindAction("JumpButton", IE_Released, this, &ARPGPlayerController::JumpButtonUp);

    InputComponent->BindAction("W", IE_Pressed, this, &ARPGPlayerController::StartMovingForward);
    InputComponent->BindAction("W", IE_Released, this, &ARPGPlayerController::StopMovingForward);

    InputComponent->BindAction("A", IE_Pressed, this, &ARPGPlayerController::StartTurningLeft);
    InputComponent->BindAction("A", IE_Released, this, &ARPGPlayerController::StopTurningLeft);

    InputComponent->BindAction("S", IE_Pressed, this, &ARPGPlayerController::StartMovingBackward);
    InputComponent->BindAction("S", IE_Released, this, &ARPGPlayerController::StopMovingBackward);

    InputComponent->BindAction("D", IE_Pressed, this, &ARPGPlayerController::StartTurningRight);
    InputComponent->BindAction("D", IE_Released, this, &ARPGPlayerController::StopTurningRight);
}

void ARPGPlayerController::WeaponButtonDown()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, 1, 0, -1, -1, -1, -1);
    }
}
void ARPGPlayerController::WeaponButtonUp()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, 0, -1, -1, -1, -1, -1);
    }
}

void ARPGPlayerController::BlockButtonDown()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, 0, 1, -1, -1, -1, -1);
    }
}
void ARPGPlayerController::BlockButtonUp()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, 0, -1, -1, -1, -1);
    }
}

void ARPGPlayerController::JumpButtonDown()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(1, -1, -1, -1, -1, -1, -1);
    }
}
void ARPGPlayerController::JumpButtonUp()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(0, -1, -1, -1, -1, -1, -1);
    }
}

void ARPGPlayerController::StartMovingForward()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, -1, -1, 1, 0);
    }
}
void ARPGPlayerController::StopMovingForward()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, -1, -1, 0, -1);
    }
}

void ARPGPlayerController::StartTurningLeft()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, 0, 1, -1, -1);
    }
}
void ARPGPlayerController::StopTurningLeft()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, -1, 0, -1, -1);
    }
}

void ARPGPlayerController::StartMovingBackward()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, -1, -1, 0, 1);
    }
}
void ARPGPlayerController::StopMovingBackward()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, -1, -1, -1, 0);
    }
}

void ARPGPlayerController::StartTurningRight()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, 1, 0, -1, -1);
    }
}
void ARPGPlayerController::StopTurningRight()
{
    ARPGCreature* pCreature = Cast<ARPGCreature>(GetPawn());
    if (pCreature)
    {
        pCreature->SetCreatureStates(-1, -1, -1, 0, -1, -1, -1);
    }
}
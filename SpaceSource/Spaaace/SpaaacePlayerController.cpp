// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SpaaacePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "SpaaaceCharacter.h"
#include "Engine/World.h"
#include "Ship/Ship.h"
#include "Complain.h"

ASpaaacePlayerController::ASpaaacePlayerController()
{
}

void ASpaaacePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ASpaaacePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ASpaaacePlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ASpaaacePlayerController::OnSetDestinationReleased);

    InputComponent->BindAxis("MoveForward", this, &ASpaaacePlayerController::MovingForward);
    InputComponent->BindAxis("MoveRight", this, &ASpaaacePlayerController::MovingRight);
    InputComponent->BindAction("Fire", IE_Pressed, this, &ASpaaacePlayerController::StartFiringWeapon);
    InputComponent->BindAction("Fire", IE_Released, this, &ASpaaacePlayerController::StopFiringWeapon);
}

void ASpaaacePlayerController::MoveToMouseCursor()
{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
}

void ASpaaacePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ASpaaacePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ASpaaacePlayerController::MovingForward(float axisValue)
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        if (MyPawn->GetClass()->IsChildOf<AShip>())
        {
            AShip* MyShip = Cast<AShip>(MyPawn);
            MyShip->MovingForwardBack(axisValue);
        }
    }
}

void ASpaaacePlayerController::MovingRight(float axisValue)
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        if (MyPawn->GetClass()->IsChildOf<AShip>())
        {
            AShip* MyShip = Cast<AShip>(MyPawn);
            MyShip->MovingLeftRight(axisValue);
        }
    }
}

void ASpaaacePlayerController::StartFiringWeapon()
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        if (MyPawn->GetClass()->IsChildOf<AShip>())
        {
            AShip* MyShip = Cast<AShip>(MyPawn);
            MyShip->StartFiringWeapon();
        }
    }
}

void ASpaaacePlayerController::StopFiringWeapon()
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        if (MyPawn->GetClass()->IsChildOf<AShip>())
        {
            AShip* MyShip = Cast<AShip>(MyPawn);
            MyShip->StopFiringWeapon();
        }
    }
}
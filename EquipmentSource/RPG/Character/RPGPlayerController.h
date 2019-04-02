// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Complain.h"
#include "RPGPlayerController.generated.h"

UCLASS()
class ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARPGPlayerController();

protected:
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

    void WeaponButtonDown();
    void WeaponButtonUp();

    void BlockButtonDown();
    void BlockButtonUp();

    void JumpButtonDown();
    void JumpButtonUp();

    void StartMovingForward();
    void StopMovingForward();

    void StartTurningLeft();
    void StopTurningLeft();

    void StartMovingBackward();
    void StopMovingBackward();

    void StartTurningRight();
    void StopTurningRight();
	// End PlayerController interface
};



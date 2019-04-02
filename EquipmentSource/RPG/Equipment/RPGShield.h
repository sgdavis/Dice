// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Complain.h"
#include "GameFramework/Actor.h"
#include "Equipment/RPGEquipment.h"
#include "RPGShield.generated.h"

UCLASS()
class ARPGShield: public ARPGEquipment
{
	GENERATED_BODY()

public:
    ARPGShield();
    void BlockedSomething(AActor* otherActor);

private:

protected:
};


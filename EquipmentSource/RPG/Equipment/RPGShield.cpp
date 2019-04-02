// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RPGShield.h"


ARPGShield::ARPGShield() : ARPGEquipment()
{
}

void ARPGShield::BlockedSomething(AActor* otherActor)
{
    CLIENTCOMPLAINT("CLANG");
}
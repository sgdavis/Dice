// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiceGameTracking.generated.h"

USTRUCT()
struct FDiceGameTracking
{
	GENERATED_BODY()

	UPROPERTY()
	int m_nCurrentNumDice;

	UPROPERTY()
	int m_nCurrentFaceValue;

	UPROPERTY()
	int m_nBetNumDice;

	UPROPERTY()
	int m_nBetFaceValue;
};

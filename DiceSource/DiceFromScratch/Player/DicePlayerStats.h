// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DicePlayerStats.generated.h"

USTRUCT()
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY()
	FString m_sName;

	UPROPERTY()
	bool m_bReady;

	UPROPERTY()
	int m_nNumDice;

	UPROPERTY()
	int m_nRoll1;

	UPROPERTY()
	int m_nRoll2;

	UPROPERTY()
	int m_nRoll3;

	UPROPERTY()
	int m_nRoll4;

	UPROPERTY()
	int m_nRoll5;

	UPROPERTY()
	bool m_bIsTurn;
};

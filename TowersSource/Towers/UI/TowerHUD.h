// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Game/TowersPlayerStats.h"
#include "TowerHUD.generated.h"

class ATowersPlayerController;

UCLASS(BlueprintType, Blueprintable)
class TOWERS_API UTowerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UTowerHUD(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnLaneHPButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnLaneSlowButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnLaneSpawnButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnMinionHPButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnMinionSpeedButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnMinionDamageButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnTowerDamageButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnTowerShotSpeedButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnTowerSpeedButtonClicked();

	ATowersPlayerController* pPlayerController;
	void SetOwnerPlayerController(ATowersPlayerController* NewPlayerContoller) { pPlayerController = NewPlayerContoller; }

	void UpdatePlayerList(const TArray<FPlayerStats>& Players);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MinionDamageUpLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MinionHPUpLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MinionSpeedUpLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TowerDamageUpLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TowerShotSpeedUpLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TowerSpeedUpLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LaneHPUpLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LaneSlowUpLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LaneSpawnLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HPLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GoldLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* IncomeLabel;
};

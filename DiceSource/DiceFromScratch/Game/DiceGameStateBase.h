// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Player/DicePlayerStats.h"
#include "Player/DicePlayerController.h"
#include "Game/DiceGameTracking.h"
#include "DiceGameStateBase.generated.h"

DECLARE_DELEGATE_OneParam(FSGPlayerListUpdatedDelegate, const TArray<FPlayerStats>&);
DECLARE_DELEGATE_OneParam(FDiceTrackingUpdatedDelegate, FDiceGameTracking);

UCLASS()
class DICEFROMSCRATCH_API ADiceGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ADiceGameStateBase();

    void SetLocalPlayerController(ADicePlayerController* player);

	void RegisterPlayerListListener(FSGPlayerListUpdatedDelegate Callback);
	void RegisterGameTrackingListener(FDiceTrackingUpdatedDelegate Callback);

	TArray<FPlayerStats> GetPlayerList() { return m_aPlayerList; }

	bool CheckPlayerNameExists(const FString& NewPlayerName);
	void AddPlayerName(const FString& NewPlayerName);
	void AddPlayerReady(const FString& NewPlayerName);

    void NextTurn(bool shouldRoll);
    void ModifyBet(int numDice, int faceValue);
    void ResetGame();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NotifyGameStart();

    int GetTotalDice(int faceValue = 0);

    FDiceGameTracking GetTracking();

	int GetCurrentPlayer();
	int GetPreviousPlayer(int playerIndex);
	void ReducePlayerDice(int playerIndex);

	FString GetPlayerName(int playerIndex);
	FString GetDiceBreakdown();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void AnnounceMessage(const FString& announcement);

	int GetNumPlayers();

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerList)
	TArray<FPlayerStats> m_aPlayerList;

	UPROPERTY(ReplicatedUsing = OnRep_GameTracking)
		FDiceGameTracking fCurrentGameStats;

	TArray<int> DiceRolls;

	FSGPlayerListUpdatedDelegate PlayerListUpdatedCallback;
	FDiceTrackingUpdatedDelegate DiceTrackingUpdatedCallback;

	ADicePlayerController* m_pPlayerController;

	int nCurrentPlayerIndex;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		void OnRep_PlayerList();

	UFUNCTION()
		void OnRep_GameTracking();

	FPlayerStats CreateAndInitPlayer(const FString& name);

	UFUNCTION(Client, Reliable, WithValidation)
		void TryStartGame();

	void RollDice();
};

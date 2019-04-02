// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/DiceLoginUI.h"
#include "UI/DiceReadyUI.h"
#include "UI/DiceGameUI.h"
#include "UI/AnnouncementUI.h"
#include "DicePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DICEFROMSCRATCH_API ADicePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADicePlayerController();

	virtual void BeginPlay() override;

    //==========LOGIN==========
	UFUNCTION()
		void TryJoinGame(const FString& NewPlayerName);

    //==========READY==========
    UFUNCTION()
        void TryStartGame();

    UFUNCTION()
        void OrderedToStartGame();

    //==========GAME==========
    UFUNCTION()
        void TryToChangeNumDice(int difference);

    UFUNCTION()
        void TryToBet();

    UFUNCTION()
        void TryToChangeFaceValue(int difference);

    UFUNCTION()
        void TryCallBluff();

    UFUNCTION()
        void TryCallSpotOn();

    //==========ANNOUNCEMENT==========
    UFUNCTION(BlueprintCallable, Category = "UI")
        void AnnounceMessage(const FString& announcement);

    UFUNCTION()
        void DismissAnnouncement();

    //==========GENERAL==========
    UFUNCTION()
        FString GetPlayerName();

    virtual void Tick(float DeltaSeconds) override;

private:
	//==========LOGIN==========
    FString m_sPlayerName = "";
    bool m_bHasShownLoginHud = false;
    bool m_bHasSubmittedLoginOptions = false;

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowLoginScreen(TSubclassOf<UDiceLoginUI> NewWidgetClass);

	// Login UI class template to load at player join.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDiceLoginUI> LoginScreenTemplate;

	// The instance of the Login UI class to allow player choice interaction.
	UPROPERTY(Transient)
		class UDiceLoginUI* DiceLoginUI;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryJoinGame(const FString& NewPlayerName);

	UFUNCTION()
		void ClientJoinResults(const bool bJoinSucceeded, const FString& failureCause);

	UFUNCTION(Client, Reliable)
		void SendClientJoinResults(const bool bJoinSucceeded, const FString& failureCause);

	//==========READY==========
	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowReadyScreen(TSubclassOf<UDiceReadyUI> NewWidgetClass);

	// Login UI class template to load at player join.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDiceReadyUI> ReadyScreenTemplate;

	// The instance of the Login UI class to allow player choice interaction.
	UPROPERTY(Transient)
		class UDiceReadyUI* DiceReadyUI;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryStartGame(const FString& playerName);

	//==========GAME==========
	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowGameScreen(TSubclassOf<UDiceGameUI> NewWidgetClass);

	// Login UI class template to load at player join.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDiceGameUI> GameScreenTemplate;

	// The instance of the Login UI class to allow player choice interaction.
	UPROPERTY(Transient)
		class UDiceGameUI* DiceGameUI;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryToBet();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryToChangeNumDice(int difference);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryToChangeFaceValue(int difference);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryCallBluff();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerTryCallSpotOn();

	//==========ANNOUNCEMENT==========
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UAnnouncementUI> AnnouncementTemplate;

	UPROPERTY(Transient)
		class UAnnouncementUI* AnnouncementUI;

    //==========GENERAL==========
	bool m_bHasRegisteredPlayerListCallback = false;
};

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Complain.h"
#include "UI/TowerHUD.h"
#include "TowersPlayerController.generated.h"

UCLASS()
class ATowersPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATowersPlayerController();

	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void TeleportPlayer(FVector location);

	bool IsReadyToTeleport();

	UFUNCTION(Server, Reliable, WithValidation)
		void RequestPlayerID();

	UFUNCTION(Client, Reliable)
		void ProvidePlayerID(int id);

	UFUNCTION(Server, Reliable, WithValidation)
		void RequestReward(int reward);

	//==========HUD==========
	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowHUD(TSubclassOf<UTowerHUD> NewWidgetClass);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UTowerHUD> HUDTemplate;

	UPROPERTY(Transient)
		class UTowerHUD* TowerHUD;

	bool bHasShowHUD;

	//==========HUD FUNCTIONS==========
	void RequestSpawn();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRequestSpawn();

	void RequestUpgrade(int upgradeIndex);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRequestUpgrade(int upgradeIndex);

	//==========GENERAL==========
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void SetPlayerIndex(int index);
	int GetPlayerIndex() { return playerIndex; }

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToDestination();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	bool bHasRegisteredPlayerListCallback;

	UPROPERTY()
	int playerIndex;

	FVector MovementDestination;
};



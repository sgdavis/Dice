// Fill out your copyright notice in the Description page of Project Settings.

#include "DicePlayerController.h"
#include "Game/DiceGameStateBase.h"
#include "Game/DiceFromScratchGameModeBase.h"

ADicePlayerController::ADicePlayerController()
{
	m_bHasShownLoginHud = false;
}

void ADicePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("BEGIN PLAY")));

	SetInputMode(FInputModeUIOnly());

	if (LoginScreenTemplate != nullptr
		&& GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy
		&& !m_bHasShownLoginHud)
	{
		ShowLoginScreen(LoginScreenTemplate);
	}
}

void ADicePlayerController::ShowLoginScreen(TSubclassOf<UDiceLoginUI> NewWidgetClass)
{
	if (DiceLoginUI != NULL)
	{
		DiceLoginUI->RemoveFromViewport();
		DiceLoginUI = NULL;
	}

	if (NewWidgetClass != NULL)
	{
		DiceLoginUI = CreateWidget<UDiceLoginUI>(GetWorld(), NewWidgetClass);
		if (DiceLoginUI != NULL)
		{
			DiceLoginUI->AddToViewport();
			DiceLoginUI->SetOwnerPlayerController(this);
		}
	}
}

void ADicePlayerController::ShowReadyScreen(TSubclassOf<UDiceReadyUI> NewWidgetClass)
{
	if (DiceLoginUI != NULL)
	{
		DiceLoginUI->RemoveFromViewport();
		DiceLoginUI = NULL;
	}

	if (NewWidgetClass != NULL)
	{
		DiceReadyUI = CreateWidget<UDiceReadyUI>(GetWorld(), NewWidgetClass);
		if (DiceReadyUI != NULL)
		{
			DiceReadyUI->AddToViewport();
			DiceReadyUI->SetOwnerPlayerController(this);

			if (ADiceGameStateBase* GS = GetWorld()->GetGameState<ADiceGameStateBase>())
			{
				// Register a listener between the GameState's ready list and the login's update function.
				FSGPlayerListUpdatedDelegate UpdateReadyListCallback;
				UpdateReadyListCallback.BindUObject(DiceReadyUI, &UDiceReadyUI::UpdateReadyList);
				GS->RegisterPlayerListListener(UpdateReadyListCallback);
			}
		}
	}
}

void ADicePlayerController::ShowGameScreen(TSubclassOf<UDiceGameUI> NewWidgetClass)
{
	if (DiceReadyUI != NULL)
	{
		DiceReadyUI->RemoveFromViewport();
		DiceReadyUI = NULL;
	}

	if (NewWidgetClass != NULL)
	{
		DiceGameUI = CreateWidget<UDiceGameUI>(GetWorld(), NewWidgetClass);
		if (DiceGameUI != NULL)
		{
			DiceGameUI->AddToViewport();
			DiceGameUI->SetOwnerPlayerController(this);

			if (ADiceGameStateBase* GS = GetWorld()->GetGameState<ADiceGameStateBase>())
			{
				// Register a listener between the GameState's ready list and the login's update function.
				FSGPlayerListUpdatedDelegate UpdateGameListCallback;
				UpdateGameListCallback.BindUObject(DiceGameUI, &UDiceGameUI::UpdateGameList);
				GS->RegisterPlayerListListener(UpdateGameListCallback);

				FDiceTrackingUpdatedDelegate UpdateGameTrackingCallback;
				UpdateGameTrackingCallback.BindUObject(DiceGameUI, &UDiceGameUI::UpdateBetValues);
				GS->RegisterGameTrackingListener(UpdateGameTrackingCallback);
			}
		}
	}
}

void ADicePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// HACK because sometimes (often?) Tick() runs (WAY) before BeginPlay(), or even before all the assigned-in-Blueprint variables have populated...
	// This appears to be an Unreal issue, not a GDK issue, as I ran into this in Vanilla Shooter as well.
	if (LoginScreenTemplate != nullptr
		&& GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy
		&& !m_bHasShownLoginHud)
	{
		m_bHasShownLoginHud = true;
		ShowLoginScreen(LoginScreenTemplate);
	}
	
	if (LoginScreenTemplate != nullptr
		&& GetNetMode() == NM_Client
		&& Role == ROLE_AutonomousProxy
		&& !m_bHasRegisteredPlayerListCallback)
	{
		if (ADiceGameStateBase* GS = GetWorld()->GetGameState<ADiceGameStateBase>())
		{
			// Register a listener between the GameState's ready list and the login's update function.
			FSGPlayerListUpdatedDelegate UpdatePlayerListCallback;
			UpdatePlayerListCallback.BindUObject(DiceLoginUI, &UDiceLoginUI::UpdatePlayerList);
			GS->RegisterPlayerListListener(UpdatePlayerListCallback);

			m_bHasRegisteredPlayerListCallback = true;

			GS->SetLocalPlayerController(this);
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("NO GAME STATE")));
		}
	}
}

//Give the server our desired player name and get registered if it is free
void ADicePlayerController::TryJoinGame(const FString& NewPlayerName)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Trying to join game (%s)"), *NewPlayerName));

	ServerTryJoinGame(NewPlayerName);

	m_sPlayerName = *NewPlayerName;
}

bool ADicePlayerController::ServerTryJoinGame_Validate(const FString& NewPlayerName)
{
	return true;
}

void ADicePlayerController::ServerTryJoinGame_Implementation(const FString& NewPlayerName)
{
	bool bJoinWasSuccessful = true;
	FString failureCause = "";
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Validate player name
	if (NewPlayerName.IsEmpty())
	{
		bJoinWasSuccessful = false;
		failureCause = "No name entered";
	}

	/*// Validate PlayerState
	if (PlayerState == nullptr
		|| !PlayerState->IsA(ATPSPlayerState::StaticClass()))
	{
		bJoinWasSuccessful = false;
		failureCause = "Invalid PlayerState";
	}*/

	// Validate the join request
	if (m_bHasSubmittedLoginOptions)
	{
		bJoinWasSuccessful = false;
		failureCause = "Already joined";
	}

	if (GM != NULL && GM->CheckPlayerNameExists(NewPlayerName))
	{
		bJoinWasSuccessful = false;
		failureCause = "Name taken";
	}

	// Inform Client as to whether or not join was accepted
	SendClientJoinResults(bJoinWasSuccessful, failureCause);

	if (bJoinWasSuccessful)
	{
		m_bHasSubmittedLoginOptions = true;

		// Set the player-selected values
		//PlayerState->SetPlayerName(NewPlayerName);

		// Add the player to the game's scoreboard.
		if (GM != NULL)
		{
			GM->NotifyPlayerJoined(NewPlayerName);
		}
	}
}

void ADicePlayerController::SendClientJoinResults_Implementation(const bool bJoinSucceeded, const FString& failureCause)
{
	ClientJoinResults(bJoinSucceeded, failureCause);
}

void ADicePlayerController::ClientJoinResults(const bool bJoinSucceeded, const FString& failureCause)
{
	check(DiceLoginUI != nullptr);

	if (bJoinSucceeded)
	{
		m_bHasSubmittedLoginOptions = true;
		ShowReadyScreen(ReadyScreenTemplate);
	}
	else
	{
		DiceLoginUI->JoinGameFailed(failureCause);
	}
}

//Try to ask the server to stop waiting for more players and just start the game
void ADicePlayerController::TryStartGame()
{
	ServerTryStartGame(m_sPlayerName);
}

bool ADicePlayerController::ServerTryStartGame_Validate(const FString& playerName)
{
	return true;
}

void ADicePlayerController::ServerTryStartGame_Implementation(const FString& playerName)
{
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Add the player to the game's scoreboard.
	if (GM != NULL)
	{
		GM->NotifyPlayerReady(playerName);
	}
}

//The server has started the game. The UI needs to move on to the game screen
void ADicePlayerController::OrderedToStartGame()
{
	ShowGameScreen(GameScreenTemplate);
}

FString ADicePlayerController::GetPlayerName()
{
	return m_sPlayerName;
}

//Let the server know we want to lock in our bet
void ADicePlayerController::TryToBet()
{
	ServerTryToBet();
}

bool ADicePlayerController::ServerTryToBet_Validate()
{
	return true;
}

void ADicePlayerController::ServerTryToBet_Implementation()
{
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Add the player to the game's scoreboard.
	if (GM != NULL)
	{
		GM->SendPlayerBet();
	}
}

//Let the server know we want to increase the face value of our bet
void ADicePlayerController::TryToChangeFaceValue(int difference)
{
	ServerTryToChangeFaceValue(difference);
}

bool ADicePlayerController::ServerTryToChangeFaceValue_Validate(int difference)
{
	return true;
}

void ADicePlayerController::ServerTryToChangeFaceValue_Implementation(int difference)
{
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Add the player to the game's scoreboard.
	if (GM != NULL)
	{
		GM->TryToChangeFaceValue(difference);
	}
}

//Let the server know we want to increase the number of dice in our bet
void ADicePlayerController::TryToChangeNumDice(int difference)
{
	ServerTryToChangeNumDice(difference);
}

bool ADicePlayerController::ServerTryToChangeNumDice_Validate(int difference)
{
	return true;
}

void ADicePlayerController::ServerTryToChangeNumDice_Implementation(int difference)
{
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Add the player to the game's scoreboard.
	if (GM != NULL)
	{
		GM->TryToChangeNumDice(difference);
	}
}

//Let the server know we want to call the previous player's bet
void ADicePlayerController::TryCallBluff()
{
	ServerTryCallBluff();
}

bool ADicePlayerController::ServerTryCallBluff_Validate()
{
	return true;
}

void ADicePlayerController::ServerTryCallBluff_Implementation()
{
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Add the player to the game's scoreboard.
	if (GM != NULL)
	{
		GM->CallBluff();
	}
}

//Let the server know we think the previous player's bet is perfectly accurate
void ADicePlayerController::TryCallSpotOn()
{
	ServerTryCallSpotOn();
}

bool ADicePlayerController::ServerTryCallSpotOn_Validate()
{
	return true;
}

void ADicePlayerController::ServerTryCallSpotOn_Implementation()
{
	ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>();

	// Add the player to the game's scoreboard.
	if (GM != NULL)
	{
		GM->CallSpotOn();
	}
}

//Pop up the announcement UI
void ADicePlayerController::AnnounceMessage(const FString& announcement)
{
	DismissAnnouncement();
	if (AnnouncementTemplate != NULL)
	{
		AnnouncementUI = CreateWidget<UAnnouncementUI>(GetWorld(), AnnouncementTemplate);
		if (AnnouncementUI != NULL)
		{
			AnnouncementUI->AddToViewport();
			AnnouncementUI->SetOwnerPlayerController(this);

			AnnouncementUI->SetAnnouncementText(announcement);
		}
	}
}

//Remove the announcement UI
void ADicePlayerController::DismissAnnouncement()
{
	if (AnnouncementUI != NULL)
	{
		AnnouncementUI->RemoveFromViewport();
		AnnouncementUI = NULL;
	}
}
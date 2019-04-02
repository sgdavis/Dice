
#include "DiceLoginUI.h"
#include "Player/DicePlayerController.h"
#include "Game/DiceGameStateBase.h"

UDiceLoginUI::UDiceLoginUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_pPlayerController = NULL;
}

//Submit the current player name to the server for registration
void UDiceLoginUI::OnJoinButtonClicked()
{
	if (ErrorMessageText)
	{
		ErrorMessageText->SetText(FText::FromString(""));
	}

	if (ADiceGameStateBase* GS = GetWorld()->GetGameState<ADiceGameStateBase>())
	{
		TArray<FPlayerStats> playerNames = GS->GetPlayerList();

		for (FPlayerStats stats : playerNames)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("GRABBED PLAYER (%s)"), *stats.m_sName));
		}
	}

	if (m_pPlayerController != NULL)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("CLICKED THE THING (%s)"), *PlayerName.ToString()));
		m_pPlayerController->TryJoinGame(PlayerName.ToString());
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("NO PLAYER CONTROLLER")));
	}
}

//Report why registration failed
void UDiceLoginUI::JoinGameFailed(FString FailureReason)
{
	if (ErrorMessageText)
	{
		ErrorMessageText->SetText(FText::FromString(FailureReason));
	}
}

//Show thel ist of players currently in the game
void UDiceLoginUI::UpdatePlayerList(const TArray<FPlayerStats>& Players)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("IN PLAYER LIST UPDATE (%d)"), Players.Num()));

	for (FPlayerStats stats : Players)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HAS PLAYER (%s)"), *stats.m_sName));
	}

	if (OtherPlayerList)
	{
		OtherPlayerList->ClearChildren();

		for (FPlayerStats stats : Players)
		{
			if (!m_mPlayerListWidgets.Contains(*stats.m_sName))
			{
				check(PlayerListWidgetTemplate);

				UDicePlayerListWidget* newPlayer = CreateWidget<UDicePlayerListWidget>(GetOwningPlayer(), PlayerListWidgetTemplate);
				newPlayer->SetName(*stats.m_sName);
				m_mPlayerListWidgets.Emplace(*stats.m_sName, newPlayer);
			}

			UDicePlayerListWidget* PlayerWidget = m_mPlayerListWidgets[*stats.m_sName];
			OtherPlayerList->AddChild(PlayerWidget);
		}
	}
}
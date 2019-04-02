// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/DiceFromScratchGameModeBase.h"
#include "Game/DiceGameStateBase.h"
#include "Game/DiceGameTracking.h"

ADiceFromScratchGameModeBase::ADiceFromScratchGameModeBase()
{
	GameStateClass = ADiceGameStateBase::StaticClass();
}

void ADiceFromScratchGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

//Ask the server's game state if a given player name is already registered
bool ADiceFromScratchGameModeBase::CheckPlayerNameExists(const FString& NewPlayerName)
{
	bool ret = false;

	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		ret = GS->CheckPlayerNameExists(NewPlayerName);
	}

	return ret;
}

//Tell the server's game state to register a players name
void ADiceFromScratchGameModeBase::NotifyPlayerJoined(const FString& NewPlayerName)
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		GS->AddPlayerName(NewPlayerName);
	}
}

//Tell the server's game state to mark a player as ready to being the game
void ADiceFromScratchGameModeBase::NotifyPlayerReady(const FString& NewPlayerName)
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		GS->AddPlayerReady(NewPlayerName);
	}
}

//Tell the server's game state to mark the game as in progress and make the rolls for the first round
void ADiceFromScratchGameModeBase::StartGame()
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		GS->NotifyGameStart();
		GS->NextTurn(true);
	}
}

//Validate the current players bet and move on to the next turn if it is valid
void ADiceFromScratchGameModeBase::SendPlayerBet()
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FDiceGameTracking trackedStats = GS->GetTracking();
		if (trackedStats.m_nBetNumDice > trackedStats.m_nCurrentNumDice || trackedStats.m_nBetFaceValue > trackedStats.m_nCurrentFaceValue)
		{
			GS->NextTurn(false);
		}
	}
}

//Validate an increase in the number of dice in the current players bet and notify all other players if it is valid
void ADiceFromScratchGameModeBase::TryToChangeNumDice(int difference)
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FDiceGameTracking trackedStats = GS->GetTracking();
		if (trackedStats.m_nBetNumDice + difference > GS->GetTotalDice())
		{
			//Report too high
		}
		else if ((trackedStats.m_nBetNumDice + difference >= trackedStats.m_nCurrentNumDice /*|| trackedStats.m_nBetFaceValue > trackedStats.m_nCurrentFaceValue*/) && trackedStats.m_nBetNumDice + difference > 0)
		{
			GS->ModifyBet(trackedStats.m_nBetNumDice + difference, trackedStats.m_nBetFaceValue);
		}
	}
}

//Validate an increase in the face value of the dice in the current players bet and notify all other players if it is valid
void ADiceFromScratchGameModeBase::TryToChangeFaceValue(int difference)
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FDiceGameTracking trackedStats = GS->GetTracking();
		if (trackedStats.m_nBetFaceValue + difference > 6)
		{
			//Report too high
		}
		else if ((trackedStats.m_nBetFaceValue + difference >= trackedStats.m_nCurrentFaceValue || trackedStats.m_nBetNumDice > trackedStats.m_nCurrentNumDice) && trackedStats.m_nBetFaceValue + difference > 0)
		{
			GS->ModifyBet(trackedStats.m_nBetNumDice, trackedStats.m_nBetFaceValue + difference);
		}
	}
}

//Check if the previous player's bet was a bluff, handle the removal of dice and move on to the next round
void ADiceFromScratchGameModeBase::CallBluff()
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FDiceGameTracking trackedStats = GS->GetTracking();

		if (GS->GetTotalDice(trackedStats.m_nCurrentFaceValue) < trackedStats.m_nCurrentNumDice)
		{
			BluffCalled(true);
		}
		else
		{
			BluffCalled(false);
		}

		GS->NextTurn(true);
	}
}

//Check if the previous player's bet was perfectly correct, handle the removal of dice and move on to the next round
void ADiceFromScratchGameModeBase::CallSpotOn()
{
	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FDiceGameTracking trackedStats = GS->GetTracking();

		if (GS->GetTotalDice(trackedStats.m_nCurrentFaceValue) == trackedStats.m_nCurrentNumDice)
		{
			SpotOnCalled(true);
		}
		else
		{
			SpotOnCalled(false);
		}

		GS->NextTurn(true);
	}
}

//Forumlate a report on the state of the previous round and the result of the called bluff, handle die number changes and report it to all players
void ADiceFromScratchGameModeBase::BluffCalled(bool success)
{
	FString announcement = "Bluff Called\n";

	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FString thim_sPlayerName = GS->GetPlayerName(GS->GetCurrentPlayer());
		FString otherPlayerName = GS->GetPlayerName(GS->GetPreviousPlayer(GS->GetCurrentPlayer()));

		FDiceGameTracking trackedStats = GS->GetTracking();
		announcement += FString::Printf(TEXT("%s bet that there are %d %d's\n"), *otherPlayerName, trackedStats.m_nCurrentNumDice, trackedStats.m_nCurrentFaceValue);
		announcement += FString::Printf(TEXT("%s has declared that this is a lie\n"), *thim_sPlayerName);

		if (success)
		{
			announcement += FString::Printf(TEXT("%s was correct! There are %d %d's\n"), *thim_sPlayerName, GS->GetTotalDice(trackedStats.m_nCurrentFaceValue), trackedStats.m_nCurrentFaceValue);
			announcement += FString::Printf(TEXT("%s will lose a die\n"), *otherPlayerName);
			GS->ReducePlayerDice(GS->GetPreviousPlayer(GS->GetCurrentPlayer()));
		}
		else
		{
			announcement += FString::Printf(TEXT("%s was wrong! There are %d %d's\n"), *thim_sPlayerName, GS->GetTotalDice(trackedStats.m_nCurrentFaceValue), trackedStats.m_nCurrentFaceValue);
			announcement += FString::Printf(TEXT("%s will lose a die\n"), *thim_sPlayerName);
			GS->ReducePlayerDice(GS->GetCurrentPlayer());
		}

		announcement += FString::Printf(TEXT("Here are the dice for that round\n\n%s\n"), *GS->GetDiceBreakdown());

		FString gameOverCheck = GameOverCheck();
		if (gameOverCheck != "")
		{
			announcement += FString::Printf(TEXT("%s is the only player with any dice left! %s wins!\n"), *gameOverCheck, *gameOverCheck);
			announcement += FString::Printf(TEXT("The Game is over and will now reset."));
			GS->ResetGame();
		}
		
		GS->AnnounceMessage(announcement);
	}
}

//Forumlate a report on the state of the previous round and the result of the called spot on, handle die number changes and report it to all players
void ADiceFromScratchGameModeBase::SpotOnCalled(bool success)
{
	FString announcement = "Spot On Called\n";

	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		FString thim_sPlayerName = GS->GetPlayerName(GS->GetCurrentPlayer());
		FString otherPlayerName = GS->GetPlayerName(GS->GetPreviousPlayer(GS->GetCurrentPlayer()));

		FDiceGameTracking trackedStats = GS->GetTracking();
		announcement += FString::Printf(TEXT("%s bet that there are %d %d's\n"), *otherPlayerName, trackedStats.m_nCurrentNumDice, trackedStats.m_nCurrentFaceValue);
		announcement += FString::Printf(TEXT("%s has declared that this is a lie\n"), *thim_sPlayerName);

		if (success)
		{
			announcement += FString::Printf(TEXT("%s was correct! There are %d %d's\n"), *thim_sPlayerName, GS->GetTotalDice(trackedStats.m_nCurrentFaceValue), trackedStats.m_nCurrentFaceValue);
			announcement += FString::Printf(TEXT("Everyone but %s will lose a die\n"), *thim_sPlayerName);
			for (int i = 0; i < GS->GetNumPlayers(); i++)
			{
				if (i != GS->GetCurrentPlayer())
				{
					GS->ReducePlayerDice(i);
				}
			}
		}
		else
		{
			announcement += FString::Printf(TEXT("%s was wrong! There are %d %d's\n"), *thim_sPlayerName, GS->GetTotalDice(trackedStats.m_nCurrentFaceValue), trackedStats.m_nCurrentFaceValue);
			announcement += FString::Printf(TEXT("%s will lose a die\n"), *thim_sPlayerName);
			GS->ReducePlayerDice(GS->GetCurrentPlayer());
		}

		announcement += FString::Printf(TEXT("Here are the dice for that round\n\n%s\n"), *GS->GetDiceBreakdown());

		FString gameOverCheck = GameOverCheck();
		if (gameOverCheck != "")
		{
			announcement += FString::Printf(TEXT("%s is the only player with any dice left! %s wins!\n"), *gameOverCheck, *gameOverCheck);
			announcement += FString::Printf(TEXT("The Game is over and will now reset."));
			GS->ResetGame();
		}

		GS->AnnounceMessage(announcement);
	}

}

//Check if there is only one player left with any dice
FString ADiceFromScratchGameModeBase::GameOverCheck()
{
	FString ret = "";
	int numWithDice = 0;

	if (ADiceGameStateBase* GS = Cast<ADiceGameStateBase>(GameState))
	{
		TArray<FPlayerStats> playerStats = GS->GetPlayerList();

		for (int i = 0; i < playerStats.Num(); i++)
		{
			if (playerStats[i].m_nNumDice > 0)
			{
				if (numWithDice == 0)
				{
					numWithDice++;
					ret = playerStats[i].m_sName;
				}
				else
				{
					ret = "";
					break;
				}
			}
		}
	}

	return ret;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/DiceGameStateBase.h"
#include "UnrealNetwork.h"
#include "Game/DiceFromScratchGameModeBase.h"

ADiceGameStateBase::ADiceGameStateBase() : Super()
{
	m_pPlayerController = NULL;

	fCurrentGameStats.m_nBetFaceValue = -1;
	fCurrentGameStats.m_nBetNumDice = -1;
	fCurrentGameStats.m_nCurrentFaceValue = -1;
	fCurrentGameStats.m_nCurrentNumDice = -1;

	nCurrentPlayerIndex = -1;
}

void ADiceGameStateBase::SetLocalPlayerController(ADicePlayerController* player)
{
	m_pPlayerController = player;
}

//Populate a new player stat structure
FPlayerStats ADiceGameStateBase::CreateAndInitPlayer(const FString& name)
{
	FPlayerStats newStats;
	newStats.m_sName = *name;
	newStats.m_bIsTurn = false;
	newStats.m_bReady = false;
	newStats.m_nNumDice = 5;
	newStats.m_nRoll1 = -1;
	newStats.m_nRoll2 = -1;
	newStats.m_nRoll3 = -1;
	newStats.m_nRoll4 = -1;
	newStats.m_nRoll5 = -1;

	return newStats;
}

//Check if we have already registered a given name
bool ADiceGameStateBase::CheckPlayerNameExists(const FString& NewPlayerName)
{
	bool ret = false;

	for (FPlayerStats stats : m_aPlayerList)
	{
		if (stats.m_sName == *NewPlayerName)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

//Register a given player name
void ADiceGameStateBase::AddPlayerName(const FString& NewPlayerName)
{
	FPlayerStats newStats = CreateAndInitPlayer(NewPlayerName);
    m_aPlayerList.Emplace(newStats);
}

//Register a player as ready to begin the game
void ADiceGameStateBase::AddPlayerReady(const FString& NewPlayerName)
{
	int numPlayersReady = 0;
	for (int i = 0; i < m_aPlayerList.Num(); i++)
	{
		if (m_aPlayerList[i].m_sName == *NewPlayerName)
		{
			FPlayerStats newStats = CreateAndInitPlayer(NewPlayerName);
			newStats.m_bReady = !m_aPlayerList[i].m_bReady;

            m_aPlayerList.RemoveAt(i);
            m_aPlayerList.EmplaceAt(i, newStats);
		}

		if (m_aPlayerList[i].m_bReady)
		{
			numPlayersReady++;
		}
	}

	if (numPlayersReady == m_aPlayerList.Num())
	{
		TryStartGame();
	}
}

int ADiceGameStateBase::GetNumPlayers()
{
	return m_aPlayerList.Num();
}

void ADiceGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADiceGameStateBase, m_aPlayerList);
	DOREPLIFETIME(ADiceGameStateBase, fCurrentGameStats);
}

//Let the UIs that care know that we have updated the player list
void ADiceGameStateBase::OnRep_PlayerList()
{
	PlayerListUpdatedCallback.ExecuteIfBound(m_aPlayerList);
}

//Let the UIs that care know that we have processed a turn
void ADiceGameStateBase::OnRep_GameTracking()
{
	DiceTrackingUpdatedCallback.ExecuteIfBound(fCurrentGameStats);
}

void ADiceGameStateBase::RegisterPlayerListListener(FSGPlayerListUpdatedDelegate Callback)
{
	check(GetNetMode() != NM_DedicatedServer);

	PlayerListUpdatedCallback = Callback;
	Callback.ExecuteIfBound(m_aPlayerList);
}

void ADiceGameStateBase::RegisterGameTrackingListener(FDiceTrackingUpdatedDelegate Callback)
{
	check(GetNetMode() != NM_DedicatedServer);

	DiceTrackingUpdatedCallback = Callback;
	Callback.ExecuteIfBound(fCurrentGameStats);
}

bool ADiceGameStateBase::TryStartGame_Validate()
{
	return true;
}

void ADiceGameStateBase::TryStartGame_Implementation()
{
	if (ADiceFromScratchGameModeBase* GM = GetWorld()->GetAuthGameMode<ADiceFromScratchGameModeBase>())
	{
		GM->StartGame();
	}
}

bool ADiceGameStateBase::NotifyGameStart_Validate()
{
	return true;
}

void ADiceGameStateBase::NotifyGameStart_Implementation()
{
	if (m_pPlayerController)
	{
		m_pPlayerController->OrderedToStartGame();
	}
}

//Lock in the last players bet and notify all players whose turn it is. If needed also roll dice for all players
void ADiceGameStateBase::NextTurn(bool shouldRoll)
{
	if (nCurrentPlayerIndex == -1)
	{
		nCurrentPlayerIndex = FMath::Rand() % m_aPlayerList.Num();

		fCurrentGameStats.m_nBetFaceValue = 1;
		fCurrentGameStats.m_nBetNumDice = 1;
	}

	fCurrentGameStats.m_nCurrentFaceValue = fCurrentGameStats.m_nBetFaceValue;
	fCurrentGameStats.m_nCurrentNumDice = fCurrentGameStats.m_nBetNumDice;

	FPlayerStats newStats = m_aPlayerList[nCurrentPlayerIndex];
	newStats.m_bIsTurn = false;
    m_aPlayerList.RemoveAt(nCurrentPlayerIndex);
    m_aPlayerList.EmplaceAt(nCurrentPlayerIndex, newStats);

	do
	{
		nCurrentPlayerIndex = (nCurrentPlayerIndex + 1) % m_aPlayerList.Num();
	} while (m_aPlayerList[nCurrentPlayerIndex].m_nNumDice == 0);

	FPlayerStats newerStats = m_aPlayerList[nCurrentPlayerIndex];
	newerStats.m_bIsTurn = true;
    m_aPlayerList.RemoveAt(nCurrentPlayerIndex);
    m_aPlayerList.EmplaceAt(nCurrentPlayerIndex, newerStats);

	if (shouldRoll)
	{
		fCurrentGameStats.m_nBetFaceValue = 1;
		fCurrentGameStats.m_nBetNumDice = 1;
		fCurrentGameStats.m_nCurrentFaceValue = -1;
		fCurrentGameStats.m_nCurrentNumDice = -1;

		RollDice();
	}
}

//Roll the proper number of dice for each player
void ADiceGameStateBase::RollDice()
{
	DiceRolls.Empty();

	for (int i = 0; i < m_aPlayerList.Num(); i++)
	{
		FPlayerStats newStats = CreateAndInitPlayer(m_aPlayerList[i].m_sName);
		newStats.m_bIsTurn = m_aPlayerList[i].m_bIsTurn;
		newStats.m_nNumDice = m_aPlayerList[i].m_nNumDice;

		if (newStats.m_nNumDice > 0)
		{
			newStats.m_nRoll1 = FMath::Rand() % 6 + 1;
			DiceRolls.Add(newStats.m_nRoll1);
		}

		if (newStats.m_nNumDice > 1)
		{
			newStats.m_nRoll2 = FMath::Rand() % 6 + 1;
			DiceRolls.Add(newStats.m_nRoll2);
		}

		if (newStats.m_nNumDice > 2)
		{
			newStats.m_nRoll3 = FMath::Rand() % 6 + 1;
			DiceRolls.Add(newStats.m_nRoll3);
		}

		if (newStats.m_nNumDice > 3)
		{
			newStats.m_nRoll4 = FMath::Rand() % 6 + 1;
			DiceRolls.Add(newStats.m_nRoll4);
		}

		if (newStats.m_nNumDice > 4)
		{
			newStats.m_nRoll5 = FMath::Rand() % 6 + 1;
			DiceRolls.Add(newStats.m_nRoll5);
		}

        m_aPlayerList.RemoveAt(i);
        m_aPlayerList.EmplaceAt(i, newStats);
	}
}

FDiceGameTracking ADiceGameStateBase::GetTracking()
{
	return fCurrentGameStats;
}

//Lock in the face value and number of dice for the current player's bet
void ADiceGameStateBase::ModifyBet(int numDice, int faceValue)
{
	fCurrentGameStats.m_nBetNumDice = numDice;
	fCurrentGameStats.m_nBetFaceValue = faceValue;
}

//Get the number of dice of a given face value, or of any value if faceValue is 0
int ADiceGameStateBase::GetTotalDice(int faceValue)
{
	if (faceValue == 0)
	{
		return DiceRolls.Num();
	}
	else
	{
		int ret = 0;

		for (int i = 0; i < DiceRolls.Num(); i++)
		{
			if (DiceRolls[i] == faceValue)
			{
				ret++;
			}
		}

		return ret;
	}
}

//Get the index for the stats of the current player
int ADiceGameStateBase::GetCurrentPlayer()
{
	return nCurrentPlayerIndex;
}

//Get the index for the stats of the player who had a turn just before the current palyer
int ADiceGameStateBase::GetPreviousPlayer(int playerIndex)
{
	int ret = playerIndex;
	do
	{
		ret = ret - 1;
		if (ret < 0)
		{
			ret = m_aPlayerList.Num() - 1;
		}
	} while (m_aPlayerList[ret].m_nNumDice == 0);

	return ret;
}

//Try to reduce the number of dice a given palyer has
void ADiceGameStateBase::ReducePlayerDice(int playerIndex)
{
	FPlayerStats newStats = m_aPlayerList[playerIndex];
	if (newStats.m_nNumDice > 0)
	{
		newStats.m_nNumDice--;
	}


    m_aPlayerList.RemoveAt(playerIndex);
    m_aPlayerList.EmplaceAt(playerIndex, newStats);
}

bool ADiceGameStateBase::AnnounceMessage_Validate(const FString& announcement)
{
	return true;
}

void ADiceGameStateBase::AnnounceMessage_Implementation(const FString& announcement)
{
	if (m_pPlayerController)
	{
		m_pPlayerController->AnnounceMessage(announcement);
	}
}

FString ADiceGameStateBase::GetPlayerName(int playerIndex)
{
	return m_aPlayerList[playerIndex].m_sName;
}

//Create a string listing all the players and the dice rolls they have for the current round
FString ADiceGameStateBase::GetDiceBreakdown()
{
	FString ret = "";

	for (int i = 0; i < m_aPlayerList.Num(); i++)
	{
		ret = ret.Append(m_aPlayerList[i].m_sName);
		ret += "\t\t";
		if (m_aPlayerList[i].m_nRoll1 != -1)
		{
			ret = ret.Append(FString::FromInt(m_aPlayerList[i].m_nRoll1));
		}
		else
		{
			ret = ret.Append("_");
		}
		ret += "   ";
		if (m_aPlayerList[i].m_nRoll2 != -1)
		{
			ret = ret.Append(FString::FromInt(m_aPlayerList[i].m_nRoll2));
		}
		else
		{
			ret = ret.Append("_");
		}
		ret += "   ";
		if (m_aPlayerList[i].m_nRoll3 != -1)
		{
			ret = ret.Append(FString::FromInt(m_aPlayerList[i].m_nRoll3));
		}
		else
		{
			ret = ret.Append("_");
		}
		ret += "   ";
		if (m_aPlayerList[i].m_nRoll4 != -1)
		{
			ret = ret.Append(FString::FromInt(m_aPlayerList[i].m_nRoll4));
		}
		else
		{
			ret = ret.Append("_");
		}
		ret += "   ";
		if (m_aPlayerList[i].m_nRoll5 != -1)
		{
			ret = ret.Append(FString::FromInt(m_aPlayerList[i].m_nRoll5));
		}
		else
		{
			ret = ret.Append("_");
		}

		ret += "\n";
	}

	return ret;
}

//Restore all players to default dice numbers
void ADiceGameStateBase::ResetGame()
{

	for (int i = 0; i < m_aPlayerList.Num(); i++)
	{
		FPlayerStats newStats = CreateAndInitPlayer(m_aPlayerList[i].m_sName);
		newStats.m_bIsTurn = m_aPlayerList[i].m_bIsTurn;

        m_aPlayerList.RemoveAt(i);
        m_aPlayerList.EmplaceAt(i, newStats);
	}
}
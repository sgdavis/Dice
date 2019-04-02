
#include "DiceGameUI.h"
#include "Player/DicePlayerController.h"
#include "Game/DiceGameStateBase.h"

UDiceGameUI::UDiceGameUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_pPlayerController = NULL;
	bYourTurn = false;
	bAnnouncedTurn = false;
}

//Try to modify the number of dice in the current bet
void UDiceGameUI::OnNumButtonClicked(int difference)
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->TryToChangeNumDice(difference);
	}
}

//Try to modify the face value of the current bet
void UDiceGameUI::OnFaceButtonClicked(int difference)
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->TryToChangeFaceValue(difference);
	}
}

//Try to submit the current bet
void UDiceGameUI::OnBetButtonClicked()
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->TryToBet();
	}
}

//Try to call the previous players bluff
void UDiceGameUI::OnBluffButtonClicked()
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->TryCallBluff();
	}
}

//Try to accept the previous players bet
void UDiceGameUI::OnCallButtonClicked()
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->TryCallSpotOn();
	}
}

//Update the list of players, their dice totals, your die rolls and hide elements if its not your turn
void UDiceGameUI::UpdateGameList(const TArray<FPlayerStats>& Players)
{
	bYourTurn = false;
	bAnnouncedTurn = false;
	bool IsYou = false;

	if (PlayerGameList)
	{
		PlayerGameList->ClearChildren();

		for (FPlayerStats stats : Players)
		{
			check(PlayerGameWidgetTemplate);

			UDiceGameListWidget* newPlayer = CreateWidget<UDiceGameListWidget>(GetOwningPlayer(), PlayerGameWidgetTemplate);
			IsYou = m_pPlayerController->GetPlayerName() == *stats.m_sName;

			newPlayer->SetName(*stats.m_sName);
			newPlayer->SetTurn(stats.m_bIsTurn);
			newPlayer->SetDieRolls(stats.m_nRoll1, stats.m_nRoll2, stats.m_nRoll3, stats.m_nRoll4, stats.m_nRoll5, IsYou);
			if (!PlayerGameWidgets.Contains(*stats.m_sName))
			{
				PlayerGameWidgets.Emplace(*stats.m_sName, newPlayer);
			}
			else
			{
				PlayerGameWidgets.FindAndRemoveChecked(*stats.m_sName);
				PlayerGameWidgets.Emplace(*stats.m_sName, newPlayer);
			}

			UDiceGameListWidget* PlayerWidget = PlayerGameWidgets[*stats.m_sName];
			PlayerGameList->AddChild(PlayerWidget);

			if (stats.m_bIsTurn && m_pPlayerController && m_pPlayerController->GetPlayerName() == *stats.m_sName)
			{
				bYourTurn = true;
			}
		}
	}

	ShowHideBetElements(bYourTurn);
}

//Show what the other players have set as their bet
void UDiceGameUI::UpdateBetValues(FDiceGameTracking betValues)
{
	if (YourBetNumDice)
	{
		YourBetNumDice->SetText(FText::FromString(FString::Printf(TEXT("%d"), betValues.m_nBetNumDice)));
	}
	if (YourBetFaceValue)
	{
		YourBetFaceValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), betValues.m_nBetFaceValue)));
	}
	if (CurrentBetNumDice)
	{
		CurrentBetNumDice->SetText(FText::FromString(FString::Printf(TEXT("%d"), betValues.m_nCurrentNumDice)));
	}
	if (CurrentBetFaceValue)
	{
		CurrentBetFaceValue->SetText(FText::FromString(FString::Printf(TEXT("%d"), betValues.m_nCurrentFaceValue)));
	}

	if (!bAnnouncedTurn)
	{
		AnnouncePlayerTurn(bYourTurn, betValues.m_nCurrentNumDice, betValues.m_nCurrentFaceValue);
	}

	ShowHideCurrentElements(betValues.m_nCurrentFaceValue > 0, bYourTurn);
}

//Print out that the current turn has ended and we've moved on
void UDiceGameUI::AnnouncePlayerTurn(bool isYourTurn, int numDice, int faceValue)
{
	if (numDice == -1)
	{
		return;
	}

	if (isYourTurn)
	{
		FString announcement = FString::Printf(TEXT("It is now your turn\nThe current bet is %d %d's"), numDice, faceValue);
		//announcement.Form; ("It is now your turn\nThe current bet is %d %d's", TArray<int>(fCurrentGameStats.m_nCurrentNumDice, fCurrentGameStats.m_nCurrentFaceValue));

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, announcement);
	}
	else
	{
		FString announcement = FString::Printf(TEXT("The current bet is %d %d's"), numDice, faceValue);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, announcement);
	}
}

//Control what elements you see based on if it's your turn
void UDiceGameUI::ShowHideBetElements(bool isYourTurn)
{
	if (YourBetLabel)
	{
		YourBetLabel->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (YourBetDiceLabel)
	{
		YourBetDiceLabel->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (YourBetSizeLabel)
	{
		YourBetSizeLabel->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (YourBetNumDice)
	{
		YourBetNumDice->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (YourBetFaceValue)
	{
		YourBetFaceValue->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (NumDicePlusButton)
	{
		NumDicePlusButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (NumDiceMinusButton)
	{
		NumDiceMinusButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (FaceValuePlusButton)
	{
		FaceValuePlusButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (FaceValueMinusButton)
	{
		FaceValueMinusButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (BetButton)
	{
		BetButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (CallBluffButton)
	{
		CallBluffButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (CallSpotOnButton)
	{
		CallSpotOnButton->SetVisibility(isYourTurn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

//Don't show the current bet if the game has just started and there hasn't been one
void UDiceGameUI::ShowHideCurrentElements(bool betExists, bool yourTurn)
{
	if (CurrentBetFaceValue)
	{
		CurrentBetFaceValue->SetVisibility(betExists ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (CurrentBetNumDice)
	{
		CurrentBetNumDice->SetVisibility(betExists ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (CallBluffButton)
	{
		CallBluffButton->SetVisibility((betExists && yourTurn) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (CallSpotOnButton)
	{
		CallSpotOnButton->SetVisibility((betExists && yourTurn) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
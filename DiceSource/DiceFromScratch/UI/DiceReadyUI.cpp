
#include "DiceReadyUI.h"
#include "Player/DicePlayerController.h"
#include "Game/DiceGameStateBase.h"

UDiceReadyUI::UDiceReadyUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_pPlayerController = NULL;
}

void UDiceReadyUI::OnPlayButtonClicked()
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->TryStartGame();
	}
}

//Show the list of players registered and if they are ready for the game to start
void UDiceReadyUI::UpdateReadyList(const TArray<FPlayerStats>& Players)
{
	if (PlayerReadyList)
	{
		PlayerReadyList->ClearChildren();

		for (FPlayerStats stats : Players)
		{
			check(PlayerReadyWidgetTemplate);

			UDiceReadyListWidget* newPlayer = CreateWidget<UDiceReadyListWidget>(GetOwningPlayer(), PlayerReadyWidgetTemplate);
			newPlayer->SetName(*stats.m_sName);
			newPlayer->SetReady(stats.m_bReady);
			if (!m_mPlayerReadyWidgets.Contains(*stats.m_sName))
			{
				m_mPlayerReadyWidgets.Emplace(*stats.m_sName, newPlayer);
			}
			else
			{
				m_mPlayerReadyWidgets.FindAndRemoveChecked(*stats.m_sName);
				m_mPlayerReadyWidgets.Emplace(*stats.m_sName, newPlayer);
			}

			UDiceReadyListWidget* PlayerWidget = m_mPlayerReadyWidgets[*stats.m_sName];
			PlayerReadyList->AddChild(PlayerWidget);
		}
	}
}
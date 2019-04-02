
#include "DiceGameListWidget.h"
#include "Components/TextBlock.h"

void UDiceGameListWidget::SetName(const FString& name)
{
	if (PlayerNameText != NULL)
	{
		PlayerNameText->SetText(FText::FromString(*name));
	}
}

void UDiceGameListWidget::SetTurn(bool isTurn)
{
	if (PlayerTurnText != NULL)
	{
		if (isTurn)
		{
			PlayerTurnText->SetText(FText::FromString("TURN"));
		}
		else
		{
			PlayerTurnText->SetText(FText::FromString(""));
		}
	}
}

void UDiceGameListWidget::SetDieRolls(int a, int b, int c, int d, int e, bool isYou)
{
	if (PlayerDie1 != NULL)
	{
		if (a > 0)
		{
			if (isYou)
			{
				PlayerDie1->SetText(FText::FromString(FString::FromInt(a)));
			}
			else
			{
				PlayerDie1->SetText(FText::FromString("X"));
			}
		}
		else
		{
			PlayerDie1->SetText(FText::FromString(""));
		}
	}
	if (PlayerDie2 != NULL)
	{
		if (b > 0)
		{
			if (isYou)
			{
				PlayerDie2->SetText(FText::FromString(FString::FromInt(b)));
			}
			else
			{
				PlayerDie2->SetText(FText::FromString("X"));
			}
		}
		else
		{
			PlayerDie2->SetText(FText::FromString(""));
		}
	}
	if (PlayerDie3 != NULL)
	{
		if (c > 0)
		{
			if (isYou)
			{
				PlayerDie3->SetText(FText::FromString(FString::FromInt(c)));
			}
			else
			{
				PlayerDie3->SetText(FText::FromString("X"));
			}
		}
		else
		{
			PlayerDie3->SetText(FText::FromString(""));
		}
	}
	if (PlayerDie4 != NULL)
	{
		if (d > 0)
		{
			if (isYou)
			{
				PlayerDie4->SetText(FText::FromString(FString::FromInt(d)));
			}
			else
			{
				PlayerDie4->SetText(FText::FromString("X"));
			}
		}
		else
		{
			PlayerDie4->SetText(FText::FromString(""));
		}
	}
	if (PlayerDie5 != NULL)
	{
		if (e > 0)
		{
			if (isYou)
			{
				PlayerDie5->SetText(FText::FromString(FString::FromInt(e)));
			}
			else
			{
				PlayerDie5->SetText(FText::FromString("X"));
			}
		}
		else
		{
			PlayerDie5->SetText(FText::FromString(""));
		}
	}
}
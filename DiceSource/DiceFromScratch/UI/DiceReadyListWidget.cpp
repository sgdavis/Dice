
#include "DiceReadyListWidget.h"
#include "Components/TextBlock.h"

void UDiceReadyListWidget::SetName(const FString& name)
{
	if (PlayerNameText != NULL)
	{
		PlayerNameText->SetText(FText::FromString(*name));
	}
}

void UDiceReadyListWidget::SetReady(bool ready)
{
	if (PlayerReadyText != NULL)
	{
		if (ready)
		{
			PlayerReadyText->SetText(FText::FromString("READY"));
		}
		else
		{
			PlayerReadyText->SetText(FText::FromString("NOT READY"));
		}
	}
}
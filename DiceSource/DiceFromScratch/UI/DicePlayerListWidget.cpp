
#include "DicePlayerListWidget.h"
#include "Components/TextBlock.h"

void UDicePlayerListWidget::SetName(const FString& name)
{
	if (PlayerNameText != NULL)
	{
		PlayerNameText->SetText(FText::FromString(*name));
	}
}
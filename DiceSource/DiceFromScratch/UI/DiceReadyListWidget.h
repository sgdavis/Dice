
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiceReadyListWidget.generated.h"

UCLASS()
class DICEFROMSCRATCH_API UDiceReadyListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetName(const FString& name);
	void SetReady(bool ready);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerReadyText;
};
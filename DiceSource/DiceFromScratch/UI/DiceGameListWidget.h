
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiceGameListWidget.generated.h"

UCLASS()
class DICEFROMSCRATCH_API UDiceGameListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetName(const FString& name);
	void SetTurn(bool turn);
	void SetDieRolls(int a, int b, int c, int d, int e, bool isYou);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerTurnText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerDie1;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerDie2;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerDie3;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerDie4;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerDie5;
};
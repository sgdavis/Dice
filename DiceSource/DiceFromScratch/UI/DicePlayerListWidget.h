
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DicePlayerListWidget.generated.h"

UCLASS()
class DICEFROMSCRATCH_API UDicePlayerListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetName(const FString& name);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Player/DicePlayerStats.h"
#include "UI/DiceReadyListWidget.h"
#include "DiceReadyUI.generated.h"

class ADicePlayerController;

UCLASS(BlueprintType, Blueprintable)
class UDiceReadyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UDiceReadyUI(const FObjectInitializer& ObjectInitializer);

	void SetOwnerPlayerController(ADicePlayerController* NewPlayerContoller) { m_pPlayerController = NewPlayerContoller; }

	UFUNCTION()
		void UpdateReadyList(const TArray<FPlayerStats>& Players);

private:
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* PlayerReadyList;

	UPROPERTY(EditDefaultsOnly, Category = "Player List")
		TSubclassOf<class UDiceReadyListWidget> PlayerReadyWidgetTemplate;

	UPROPERTY(VisibleAnywhere, Category = "Player List")
		TMap<FString, class UDiceReadyListWidget*> m_mPlayerReadyWidgets;

	// Submit Name
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnPlayButtonClicked();

	ADicePlayerController* m_pPlayerController;
};
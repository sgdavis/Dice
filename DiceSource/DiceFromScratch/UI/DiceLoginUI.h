
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "EditableTextBox.h"
#include "Player/DicePlayerStats.h"
#include "UI/DicePlayerListWidget.h"
#include "DiceLoginUI.generated.h"

class ADicePlayerController;

UCLASS(BlueprintType, Blueprintable)
class UDiceLoginUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UDiceLoginUI(const FObjectInitializer& ObjectInitializer);

	void SetOwnerPlayerController(ADicePlayerController* NewPlayerContoller) { m_pPlayerController = NewPlayerContoller; }

	// Name Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player_Name)
		FText PlayerName;

	UFUNCTION()
		void JoinGameFailed(FString FailureReason);

	UFUNCTION()
		void UpdatePlayerList(const TArray<FPlayerStats>& Players);

private:
	// Error Message
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ErrorMessageText;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* OtherPlayerList;

	UPROPERTY(EditDefaultsOnly, Category = "Player List")
		TSubclassOf<class UDicePlayerListWidget> PlayerListWidgetTemplate;

	UPROPERTY(VisibleAnywhere, Category = "Player List")
		TMap<FString, class UDicePlayerListWidget*> m_mPlayerListWidgets;

	// Submit Name
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnJoinButtonClicked();

	ADicePlayerController* m_pPlayerController;
};
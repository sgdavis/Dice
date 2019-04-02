
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Player/DicePlayerStats.h"
#include "Game/DiceGameTracking.h"
#include "UI/DiceGameListWidget.h"
#include "DiceGameUI.generated.h"

class ADicePlayerController;

UCLASS(BlueprintType, Blueprintable)
class UDiceGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UDiceGameUI(const FObjectInitializer& ObjectInitializer);

	//==========GENERAL==========
	void SetOwnerPlayerController(ADicePlayerController* NewPlayerContoller) { m_pPlayerController = NewPlayerContoller; }

	UFUNCTION()
		void UpdateGameList(const TArray<FPlayerStats>& Players);

	UFUNCTION()
		void UpdateBetValues(FDiceGameTracking betValues);

	UFUNCTION()
		void AnnouncePlayerTurn(bool isYourTurn, int numDice, int faceValue);

private:
    UPROPERTY(EditDefaultsOnly, Category = "Player List")
        TSubclassOf<class UDiceGameListWidget> PlayerGameWidgetTemplate;

    UPROPERTY(VisibleAnywhere, Category = "Player List")
        TMap<FString, class UDiceGameListWidget*> PlayerGameWidgets;

    //==========YOUR INTERFACE==========
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* YourBetNumDice;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* YourBetFaceValue;

    UPROPERTY(meta = (BindWidget))
        class UButton* NumDicePlusButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* NumDiceMinusButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* FaceValuePlusButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* FaceValueMinusButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* BetButton;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* YourBetLabel;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* YourBetDiceLabel;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* YourBetSizeLabel;

    UFUNCTION(BlueprintCallable, Category = "Widgets")
        void OnNumButtonClicked(int difference);

    UFUNCTION(BlueprintCallable, Category = "Widgets")
        void OnFaceButtonClicked(int difference);

    UFUNCTION(BlueprintCallable, Category = "Widgets")
        void OnBetButtonClicked();

    bool bYourTurn = false;
    bool bAnnouncedTurn = false;

    //==========CURRENT INTERFACE==========
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* CurrentBetNumDice;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* CurrentBetFaceValue;

    UPROPERTY(meta = (BindWidget))
        class UButton* CallBluffButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* CallSpotOnButton;

    UFUNCTION(BlueprintCallable, Category = "Widgets")
        void OnBluffButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Widgets")
        void OnCallButtonClicked();

    //==========GENERAL==========
    ADicePlayerController* m_pPlayerController;

    UPROPERTY(meta = (BindWidget))
        class UPanelWidget* PlayerGameList;

    UFUNCTION()
        void ShowHideBetElements(bool isYourTurn);

    UFUNCTION()
        void ShowHideCurrentElements(bool betExists, bool yourTurn);
};
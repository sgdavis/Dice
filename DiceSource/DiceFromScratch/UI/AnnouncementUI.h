
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AnnouncementUI.generated.h"

class ADicePlayerController;

UCLASS(BlueprintType, Blueprintable)
class UAnnouncementUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UAnnouncementUI(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		void SetAnnouncementText(const FString& announcement);

	void SetOwnerPlayerController(ADicePlayerController* NewPlayerContoller) { m_pPlayerController = NewPlayerContoller; }

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AnnouncementText;

	// Submit Name
	UFUNCTION(BlueprintCallable, Category = "Widgets")
		void OnOKButtonClicked();

	ADicePlayerController* m_pPlayerController;
};
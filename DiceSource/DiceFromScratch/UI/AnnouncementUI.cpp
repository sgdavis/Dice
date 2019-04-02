
#include "UI/AnnouncementUI.h"
#include "Player/DicePlayerController.h"

UAnnouncementUI::UAnnouncementUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_pPlayerController = NULL;
}

void UAnnouncementUI::OnOKButtonClicked()
{
	if (m_pPlayerController != NULL)
	{
		m_pPlayerController->DismissAnnouncement();
	}
}

void UAnnouncementUI::SetAnnouncementText(const FString& announcement)
{
	if (AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(*announcement));
	}
}
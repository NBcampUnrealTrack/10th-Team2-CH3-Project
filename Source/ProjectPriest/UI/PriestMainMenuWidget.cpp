#include "PriestMainMenuWidget.h"
#include "PriestMainMenuPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"

void UPriestMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	ShowStatusMessage(FText::GetEmpty());
	ShowTitle();

	if (!MenuSwitcher || MenuSwitcher->GetNumWidgets() != 3
		|| !LobbySwitcher || LobbySwitcher->GetNumWidgets() != 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Priest Menu: MenuSwitcher needs Title/Lobby/Credits; LobbySwitcher needs Region/Equipment."));
	}
}

void UPriestMainMenuWidget::ShowTitle()
{
	CurrentPage = EPriestMenuPage::Title;
	CurrentTab = EPriestLobbyTab::Region;
	RefreshPage();
}

void UPriestMainMenuWidget::ShowRegions()
{
	CurrentPage = EPriestMenuPage::Lobby;
	CurrentTab = EPriestLobbyTab::Region;
	RefreshPage();
}

void UPriestMainMenuWidget::ShowEquipment()
{
	CurrentPage = EPriestMenuPage::Lobby;
	CurrentTab = EPriestLobbyTab::Equipment;
	RefreshPage();
}

void UPriestMainMenuWidget::ShowCredits()
{
	CurrentPage = EPriestMenuPage::Credits;
	RefreshPage();
}

void UPriestMainMenuWidget::SwitchLobbyTab()
{
	if (CurrentPage != EPriestMenuPage::Lobby)
	{
		return;
	}

	if (CurrentTab == EPriestLobbyTab::Region)
	{
		ShowEquipment();
	}
	else
	{
		ShowRegions();
	}
}

void UPriestMainMenuWidget::RefreshPage()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(static_cast<int32>(CurrentPage));
	}
	if (LobbySwitcher)
	{
		LobbySwitcher->SetActiveWidgetIndex(static_cast<int32>(CurrentTab));
	}
	ShowStatusMessage(FText::GetEmpty());
	OnMenuStateChanged(CurrentPage, CurrentTab);
}

bool UPriestMainMenuWidget::StartStageOne()
{
	if (CurrentPage != EPriestMenuPage::Lobby || CurrentTab != EPriestLobbyTab::Region)
	{
		return false;
	}

	APriestMainMenuPlayerController* Controller = Cast<APriestMainMenuPlayerController>(GetOwningPlayer());
	if (!Controller)
	{
		return false;
	}
	return Controller->StartStageOne();
}

void UPriestMainMenuWidget::QuitGame()
{
	APlayerController* Controller = GetOwningPlayer();
	if (!Controller)
	{
		return;
	}
	UKismetSystemLibrary::QuitGame(this, Controller, EQuitPreference::Quit, false);
}

void UPriestMainMenuWidget::ShowStatusMessage(const FText& Message)
{
	if (MenuStatusText)
	{
		MenuStatusText->SetText(Message);
	}
}

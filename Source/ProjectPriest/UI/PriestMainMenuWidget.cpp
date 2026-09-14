#include "PriestMainMenuWidget.h"
#include "PriestMainMenuPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "PriestMenuMvc.h"
#include "UObject/StrongObjectPtr.h"

void UPriestMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	ShowStatusMessage(FText::GetEmpty());
	RefreshPage();

	if (!MenuSwitcher || MenuSwitcher->GetNumWidgets() != 3
		|| !LobbySwitcher || LobbySwitcher->GetNumWidgets() != 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Priest Menu: MenuSwitcher needs Title/Lobby/Credits; LobbySwitcher needs Region/Equipment."));
	}
}

void UPriestMainMenuWidget::ShowTitle()
{
    SendRequest(EPriestMenuAction::Title);
}
void UPriestMainMenuWidget::ShowRegions()
{
    SendRequest(EPriestMenuAction::Regions);
}
void UPriestMainMenuWidget::ShowEquipment()
{
    SendRequest(EPriestMenuAction::Equipment);
}
void UPriestMainMenuWidget::ShowCredits()
{
    SendRequest(EPriestMenuAction::Credits);
}
void UPriestMainMenuWidget::SwitchLobbyTab()
{
    SendRequest(EPriestMenuAction::SwitchTab);
}
void UPriestMainMenuWidget::ApplyMenuState(EPriestMenuPage Page, EPriestLobbyTab Tab)
{
    CurrentPage = Page;
    CurrentTab = Tab;
    RefreshPage();
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
    return SendRequest(EPriestMenuAction::StartStage);
}
void UPriestMainMenuWidget::QuitGame()
{
    SendRequest(EPriestMenuAction::Quit);
}
bool UPriestMainMenuWidget::SendRequest(EPriestMenuAction Action)
{
    TStrongObjectPtr<UPriestMenuRequest> Request(NewObject<UPriestMenuRequest>());
    Request->Action = Action;
    InvokeViewEvent(EViewEventType::ButtonClicked, Request.Get());
    return Request->bAccepted;
}
FDelegateHandle UPriestMainMenuWidget::AddListener(UMvcControl* Control)
{
    return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent);
}
void UPriestMainMenuWidget::RemoveListener(FDelegateHandle Handle)
{
    Listener.Remove(Handle);
}
void UPriestMainMenuWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter)
{
    Listener.Broadcast(this, EventType, Parameter);
}
void UPriestMainMenuWidget::ShowStatusMessage(const FText& Message)
{
	if (MenuStatusText)
	{
		MenuStatusText->SetText(Message);
	}
}

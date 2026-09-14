#include "PriestMenuMvc.h"
#include "PriestMainMenuPlayerController.h"
void UPriestMenuModel::SetState(EPriestMenuPage NewPage, EPriestLobbyTab NewTab)
{
    Page = NewPage;
    Tab = NewTab;
    InvokePropertyChanged(0);
}
FDelegateHandle UPriestMenuModel::AddListener(UMvcControl* Control) { return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged); }
void UPriestMenuModel::RemoveListener(FDelegateHandle Handle) { Changed.Remove(Handle); }
void UPriestMenuModel::InvokePropertyChanged(uint8 PropertyName) { Changed.Broadcast(this, PropertyName); }
void UPriestMenuController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    auto* Model = GetModel<UPriestMenuModel>();
    auto* View = GetView<UPriestMainMenuWidget>();
    if (Model && View && InModel == Model) View->ApplyMenuState(Model->GetPage(), Model->GetTab());
}
void UPriestMenuController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    auto* Model = GetModel<UPriestMenuModel>();
    auto* View = GetView<UPriestMainMenuWidget>();
    auto* Request = Cast<UPriestMenuRequest>(Parameter);
    if (!Model || !View || InView != View || !Request || EventType != EViewEventType::ButtonClicked) return;
    auto* Owner = Cast<APriestMainMenuPlayerController>(View->GetOwningPlayer());
    if (!Owner || !Owner->CanProcessMenuRequest()) return;
    Request->bAccepted = true;
    switch (Request->Action)
    {
    case EPriestMenuAction::Title: Model->SetState(EPriestMenuPage::Title, EPriestLobbyTab::Region); break;
    case EPriestMenuAction::Regions: Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Region); break;
    case EPriestMenuAction::Equipment: Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Equipment); break;
    case EPriestMenuAction::Credits: Model->SetState(EPriestMenuPage::Credits, Model->GetTab()); break;
    case EPriestMenuAction::SwitchTab:
        Request->bAccepted = Model->GetPage() == EPriestMenuPage::Lobby;
        if (Request->bAccepted) Model->SetState(EPriestMenuPage::Lobby, Model->GetTab() == EPriestLobbyTab::Region ? EPriestLobbyTab::Equipment : EPriestLobbyTab::Region);
        break;
    case EPriestMenuAction::StartStage:
        Request->bAccepted = Model->GetPage() == EPriestMenuPage::Lobby && Model->GetTab() == EPriestLobbyTab::Region && Owner->StartStageOne();
        break;
    case EPriestMenuAction::Quit: Owner->RequestQuitGame(); break;
    default: Request->bAccepted = false; break;
    }
}

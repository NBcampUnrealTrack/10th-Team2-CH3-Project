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
    UPriestMenuModel* Model = GetModel<UPriestMenuModel>();
    UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();
    if (!IsValid(Model))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Menu model is invalid. Connect the model before updating the menu."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (!IsValid(View))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Menu view is invalid. Connect the view before updating the menu."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (InModel != Model)
    {
        return;
    }
    View->ApplyMenuState(Model->GetPage(), Model->GetTab());
}
void UPriestMenuController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    UPriestMenuModel* Model = GetModel<UPriestMenuModel>();
    UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();
    UPriestMenuRequest* Request = Cast<UPriestMenuRequest>(Parameter);
    if (!IsValid(Model))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Menu model is invalid."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (!IsValid(View))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Menu view is invalid."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (InView != View || EventType != EViewEventType::ButtonClicked)
    {
        return;
    }
    if (!IsValid(Request))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Menu button event requires a PriestMenuRequest parameter."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    APriestMainMenuPlayerController* Owner = Cast<APriestMainMenuPlayerController>(View->GetOwningPlayer());
    if (!Owner)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Menu view must be owned by PriestMainMenuPlayerController."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (!Owner->CanProcessMenuRequest())
    {
        return;
    }
    Request->bAccepted = true;
    switch (Request->Action)
    {
    case EPriestMenuAction::Title: Model->SetState(EPriestMenuPage::Title, EPriestLobbyTab::Region); break;
    case EPriestMenuAction::Regions: Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Region); break;
    case EPriestMenuAction::Equipment: Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Equipment); break;
    case EPriestMenuAction::Credits: Model->SetState(EPriestMenuPage::Credits, Model->GetTab()); break;
    case EPriestMenuAction::SwitchTab:
        Request->bAccepted = Model->GetPage() == EPriestMenuPage::Lobby;
        if (Request->bAccepted)
        {
            Model->SetState(EPriestMenuPage::Lobby, Model->GetTab() == EPriestLobbyTab::Region ? EPriestLobbyTab::Equipment : EPriestLobbyTab::Region);
        }
        break;
    case EPriestMenuAction::StartStage:
        Request->bAccepted = Model->GetPage() == EPriestMenuPage::Lobby && Model->GetTab() == EPriestLobbyTab::Region && Owner->StartStageOne();
        break;
    case EPriestMenuAction::Quit: Owner->RequestQuitGame(); break;
    default: Request->bAccepted = false; break;
    }
}

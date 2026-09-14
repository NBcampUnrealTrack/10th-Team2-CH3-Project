#include "PriestMenuMvc.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "PriestMainMenuPlayerController.h"
void UPriestMenuModel::SetState(EPriestMenuPage NewPage, EPriestLobbyTab NewTab)
{
    Page = NewPage;
    Tab = NewTab;
    InvokePropertyChanged(0);
}
FDelegateHandle UPriestMenuModel::AddListener(UMvcControl* Control)
{
    return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}
void UPriestMenuModel::RemoveListener(FDelegateHandle Handle)
{
    Changed.Remove(Handle);
}
void UPriestMenuModel::InvokePropertyChanged(uint8 PropertyName)
{
    Changed.Broadcast(this, PropertyName);
}
void UPriestMenuController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestMenuModel* Model = GetModel<UPriestMenuModel>();
    UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();
    JASSERT((IsValid(Model)), "%hs [%s]: Menu model is invalid. Connect the model before updating the menu.", __FUNCTION__, *GetNameSafe(this));
    JASSERT((IsValid(View)), "%hs [%s]: Menu view is invalid. Connect the view before updating the menu.", __FUNCTION__, *GetNameSafe(this));
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
    JASSERT((IsValid(Model)), "%hs [%s]: Menu model is invalid.", __FUNCTION__, *GetNameSafe(this));
    JASSERT((IsValid(View)), "%hs [%s]: Menu view is invalid.", __FUNCTION__, *GetNameSafe(this));
    if (InView != View || EventType != EViewEventType::ButtonClicked)
    {
        return;
    }
    JASSERT((IsValid(Request)), "%hs [%s]: Menu button event requires a PriestMenuRequest parameter.", __FUNCTION__, *GetNameSafe(this));
    APriestMainMenuPlayerController* Owner = Cast<APriestMainMenuPlayerController>(View->GetOwningPlayer());
    JASSERT((IsValid(Owner)), "%hs [%s]: Menu view must be owned by PriestMainMenuPlayerController.", __FUNCTION__, *GetNameSafe(this));
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

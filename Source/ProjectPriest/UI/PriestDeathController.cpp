#include "PriestDeathController.h"
#include "PriestDeathWidget.h"
#include "PriestUIManager.h"
#include "PriestCombatModel.h"
#include "IngamePlayerController.h"

void UPriestDeathController::Initialize(UPriestCombatModel* Model, UPriestUIManager* UI, AIngamePlayerController* InPlayerController)
{
    Disconnect();
    UIManager = UI;
    PlayerController = InPlayerController;
    if (PlayerController.IsValid())
    {
        TravelFailedHandle = PlayerController->OnDeathTravelFailed.AddUObject(this, &UPriestDeathController::HandleTravelFailed);
    }
    SetModel(Model);
    HandleModelChanged(Model, 0);
}

void UPriestDeathController::Disconnect()
{
    if (PlayerController.IsValid()) PlayerController->OnDeathTravelFailed.Remove(TravelFailedHandle);
    TravelFailedHandle.Reset();
    LeaveDeath(false);
    Super::Disconnect();
    PlayerController.Reset();
    UIManager.Reset();
}

void UPriestDeathController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestCombatModel* Model = GetModel<UPriestCombatModel>();
    if (!Model || InModel != Model) return;
    const bool bDead = Model->IsPlayerDead();
    if (bDead == bDeathActive) return;
    if (bDead) EnterDeath();
    else LeaveDeath(true);
}

void UPriestDeathController::EnterDeath()
{
    bDeathActive = true;
    UPriestDeathWidget* View = nullptr;
    if (UIManager.IsValid())
    {
        bRestoreHUD = UIManager->IsHUDDisplayed();
        UIManager->HideHUD();
        View = UIManager->ShowDeathScreen(PlayerController.Get());
    }
    SetView(View);
    if (PlayerController.IsValid()) PlayerController->SetDeathInput(true, View);
    if (View)
    {
        View->SetBusy(false);
        View->ShowStatus(FText::GetEmpty());
        View->FocusRestart();
    }
}

void UPriestDeathController::LeaveDeath(bool bRestorePreviousHUD)
{
    if (!bDeathActive) return;
    bDeathActive = false;
    bTravelPending = false;
    SetView(nullptr);
    if (UIManager.IsValid()) UIManager->HideDeathScreen();
    if (PlayerController.IsValid()) PlayerController->SetDeathInput(false, nullptr);
    if (bRestorePreviousHUD && bRestoreHUD && UIManager.IsValid()) UIManager->RestoreHUD();
    bRestoreHUD = false;
}

void UPriestDeathController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    UPriestCombatModel* Model = GetModel<UPriestCombatModel>();
    UPriestDeathWidget* View = GetView<UPriestDeathWidget>();
    UPriestDeathRequest* Request = Cast<UPriestDeathRequest>(Parameter);
    if (!bDeathActive || bTravelPending || !Model || !Model->IsPlayerDead() || !View || InView != View
        || !Request || EventType != EViewEventType::ButtonClicked || !PlayerController.IsValid()) return;

    bTravelPending = true;
    PresentTravelStatus(true, NSLOCTEXT("PriestDeath", "Loading", "불러오는 중..."));
    FText Error;
    if (!PlayerController->ExecuteDeathTravel(Request->bRestart, Error)) HandleTravelFailed(Error);
}

void UPriestDeathController::HandleTravelFailed(const FText& Message)
{
    if (!bDeathActive || !bTravelPending) return;
    bTravelPending = false;
    PresentTravelStatus(false, Message);
}

void UPriestDeathController::PresentTravelStatus(bool bBusy, const FText& Message)
{
    if (UPriestDeathWidget* View = GetView<UPriestDeathWidget>())
    {
        View->SetBusy(bBusy);
        View->ShowStatus(Message);
        if (!bBusy) View->FocusRestart();
    }
}

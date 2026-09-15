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
        TravelFailedHandle = PlayerController->OnResultTravelFailed.AddUObject(this, &UPriestDeathController::HandleTravelFailed);
    }
    if (UI && !IsValid(InPlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: UI presentation was requested without a valid IngamePlayerController."), __FUNCTION__, *GetNameSafe(this));
    }
    SetModel(Model);
    HandleModelChanged(Model, 0);
}

void UPriestDeathController::Disconnect()
{
    if (PlayerController.IsValid())
    {
        PlayerController->OnResultTravelFailed.Remove(TravelFailedHandle);
    }
    TravelFailedHandle.Reset();
    LeaveDeath(false);
    Super::Disconnect();
    PlayerController.Reset();
    UIManager.Reset();
}

void UPriestDeathController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestCombatModel* Model = GetModel<UPriestCombatModel>();
    if (!Model || InModel != Model)
    {
        return;
    }
    const bool bDead = Model->IsPlayerDead();
    if (bDead == bDeathActive)
    {
        return;
    }
    if (bDead)
    {
        EnterDeath();
    }
    else
    {
        LeaveDeath(true);
    }
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
    if (PlayerController.IsValid())
    {
        PlayerController->SetResultInput(true, View);
    }
    if (View)
    {
        View->SetBusy(false);
        View->ShowStatus(FText::GetEmpty());
        View->FocusRestart();
    }
}

void UPriestDeathController::LeaveDeath(bool bRestorePreviousHUD)
{
    if (!bDeathActive)
    {
        return;
    }
    bDeathActive = false;
    bTravelPending = false;
    SetView(nullptr);
    if (UIManager.IsValid())
    {
        UIManager->HideDeathScreen();
    }
    if (PlayerController.IsValid())
    {
        PlayerController->SetResultInput(false, nullptr);
    }
    if (bRestorePreviousHUD && bRestoreHUD && UIManager.IsValid())
    {
        UIManager->RestoreHUD();
    }
    bRestoreHUD = false;
}

void UPriestDeathController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    UPriestCombatModel* Model = GetModel<UPriestCombatModel>();
    UPriestDeathWidget* View = GetView<UPriestDeathWidget>();
    UPriestDeathRequest* Request = Cast<UPriestDeathRequest>(Parameter);
    if (!bDeathActive || bTravelPending || EventType != EViewEventType::ButtonClicked)
    {
        return;
    }
    if (!IsValid(Model) || !IsValid(View))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Active Death UI lost its model or view."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (InView != View || !Model->IsPlayerDead())
    {
        return;
    }
    if (!IsValid(Request))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Button event requires a PriestDeathRequest parameter."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (!PlayerController.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Owning IngamePlayerController is unavailable; map travel cannot be requested."), __FUNCTION__, *GetNameSafe(this));
        View->ShowStatus(NSLOCTEXT("PriestUI", "MissingOwner", "Unable to return to the menu: player controller is unavailable."));
        return;
    }

    bTravelPending = true;
    PresentTravelStatus(true, NSLOCTEXT("PriestDeath", "Loading", "Loading..."));
    FText Error;
    if (!PlayerController->ExecuteResultTravel(Request->bRestart, Error))
    {
        HandleTravelFailed(Error);
    }
}

void UPriestDeathController::HandleTravelFailed(const FText& Message)
{
    if (!bDeathActive || !bTravelPending)
    {
        return;
    }
    bTravelPending = false;
    PresentTravelStatus(false, Message);
}

void UPriestDeathController::PresentTravelStatus(bool bBusy, const FText& Message)
{
    UPriestDeathWidget* View = GetView<UPriestDeathWidget>();
    if (!IsValid(View))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Death view is invalid. Cannot display travel status."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    View->SetBusy(bBusy);
    View->ShowStatus(Message);
    if (!bBusy)
    {
        View->FocusRestart();
    }
}

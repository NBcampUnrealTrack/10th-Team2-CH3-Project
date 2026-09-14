#include "PriestStageClearController.h"
#include "PriestStageClearWidget.h"
#include "PriestUIManager.h"
#include "IngameGameState.h"
#include "IngamePlayerController.h"

void UPriestStageClearController::Initialize(AIngameGameState* Model, UPriestUIManager* UI, AIngamePlayerController* InPlayerController)
{
    Disconnect();
    UIManager = UI;
    PlayerController = InPlayerController;
    if (PlayerController.IsValid())
    {
        TravelFailedHandle = PlayerController->OnResultTravelFailed.AddUObject(this, &UPriestStageClearController::HandleTravelFailed);
    }
    if (UI && !IsValid(InPlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: UI presentation was requested without a valid IngamePlayerController."), __FUNCTION__, *GetNameSafe(this));
    }
    SetModel(Model);
    HandleModelChanged(Model, 0);
}

void UPriestStageClearController::Disconnect()
{
    if (PlayerController.IsValid())
    {
        PlayerController->OnResultTravelFailed.Remove(TravelFailedHandle);
    }
    TravelFailedHandle.Reset();
    LeaveStageClear(false);
    Super::Disconnect();
    PlayerController.Reset();
    UIManager.Reset();
}

void UPriestStageClearController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    AIngameGameState* Model = GetModel<AIngameGameState>();
    if (!Model || InModel != Model)
    {
        return;
    }
    const bool bCleared = Model->HasStageCleared();
    if (bCleared == bStageClearActive)
    {
        return;
    }
    if (bCleared)
    {
        EnterStageClear();
    }
    else
    {
        LeaveStageClear(true);
    }
}

void UPriestStageClearController::EnterStageClear()
{
    bStageClearActive = true;
    UPriestStageClearWidget* View = nullptr;
    if (UIManager.IsValid())
    {
        bRestoreHUD = UIManager->IsHUDDisplayed();
        UIManager->HideHUD();
        View = UIManager->ShowStageClearScreen(PlayerController.Get());
    }
    SetView(View);
    if (PlayerController.IsValid())
    {
        PlayerController->SetResultInput(true, View);
    }
    if (View)
    {
        View->SetClearTime(GetModel<AIngameGameState>()->GetElapsedTime());
        View->SetBusy(false);
        View->ShowStatus(FText::GetEmpty());
        View->FocusMainMenu();
    }
}

void UPriestStageClearController::LeaveStageClear(bool bRestorePreviousHUD)
{
    if (!bStageClearActive)
    {
        return;
    }
    bStageClearActive = false;
    bTravelPending = false;
    SetView(nullptr);
    if (UIManager.IsValid())
    {
        UIManager->HideStageClearScreen();
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

void UPriestStageClearController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    AIngameGameState* Model = GetModel<AIngameGameState>();
    UPriestStageClearWidget* View = GetView<UPriestStageClearWidget>();
    UPriestStageClearRequest* Request = Cast<UPriestStageClearRequest>(Parameter);
    if (!bStageClearActive || bTravelPending || EventType != EViewEventType::ButtonClicked)
    {
        return;
    }
    if (!IsValid(Model) || !IsValid(View))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Active StageClear UI lost its model or view."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (InView != View || !Model->HasStageCleared())
    {
        return;
    }
    if (!IsValid(Request))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Button event requires a PriestStageClearRequest parameter."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (!PlayerController.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Owning IngamePlayerController is unavailable; map travel cannot be requested."), __FUNCTION__, *GetNameSafe(this));
        View->ShowStatus(NSLOCTEXT("PriestUI", "MissingOwner", "Unable to return to the menu: player controller is unavailable."));
        return;
    }

    bTravelPending = true;
    PresentTravelStatus(true, NSLOCTEXT("PriestStageClear", "Loading", "Loading..."));
    FText Error;
    if (!PlayerController->ExecuteResultTravel(false, Error))
    {
        HandleTravelFailed(Error);
    }
}

void UPriestStageClearController::HandleTravelFailed(const FText& Message)
{
    if (!bStageClearActive || !bTravelPending)
    {
        return;
    }
    bTravelPending = false;
    PresentTravelStatus(false, Message);
}

void UPriestStageClearController::PresentTravelStatus(bool bBusy, const FText& Message)
{
    UPriestStageClearWidget* View = GetView<UPriestStageClearWidget>();
    if (!IsValid(View))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: StageClear view is invalid. Cannot display travel status."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    View->SetBusy(bBusy);
    View->ShowStatus(Message);
    if (!bBusy)
    {
        View->FocusMainMenu();
    }
}

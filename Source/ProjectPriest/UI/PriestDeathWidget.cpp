#include "PriestDeathWidget.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "MvcControl.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/StrongObjectPtr.h"
bool UPriestDeathWidget::Initialize()
{
    if (bBindingsReady)
    {
        return true;
    }
    JASSERT_BOOL((Super::Initialize()), "%hs: Base widget initialization failed", __FUNCTION__);
    JASSERT_BOOL((IsValid(RestartButton)), "%hs: Missing required RestartButton binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(MainMenuButton)), "%hs: Missing required MainMenuButton binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(StatusText)), "%hs: Missing required StatusText binding", __FUNCTION__);
    bBindingsReady = true;
    SetIsFocusable(true);
    RestartButton->OnClicked.AddUniqueDynamic(this, &UPriestDeathWidget::RequestRestart);
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPriestDeathWidget::RequestMainMenu);
    ShowStatus(FText::GetEmpty());
    return true;
}
void UPriestDeathWidget::SetBusy(bool bBusy)
{
    RestartButton->SetIsEnabled(!bBusy);
    MainMenuButton->SetIsEnabled(!bBusy);
}
void UPriestDeathWidget::ShowStatus(const FText& Message)
{
    StatusText->SetText(Message);
}
void UPriestDeathWidget::FocusRestart()
{
    if (GetOwningPlayer())
    {
        RestartButton->SetUserFocus(GetOwningPlayer());
    }
}
void UPriestDeathWidget::RequestRestart()
{
    SendRequest(true);
}
void UPriestDeathWidget::RequestMainMenu()
{
    SendRequest(false);
}
void UPriestDeathWidget::SendRequest(bool bRestart)
{
    TStrongObjectPtr<UPriestDeathRequest> Request(NewObject<UPriestDeathRequest>());
    Request->bRestart = bRestart;
    InvokeViewEvent(EViewEventType::ButtonClicked, Request.Get());
}
FDelegateHandle UPriestDeathWidget::AddListener(UMvcControl* Control)
{
    return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent);
}
void UPriestDeathWidget::RemoveListener(FDelegateHandle Handle)
{
    Listener.Remove(Handle);
}
void UPriestDeathWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter)
{
    Listener.Broadcast(this, EventType, Parameter);
}

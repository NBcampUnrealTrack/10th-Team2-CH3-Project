#include "PriestDeathWidget.h"
#include "MvcControl.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/StrongObjectPtr.h"
void UPriestDeathWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    SetIsFocusable(true);
    if (RestartButton) RestartButton->OnClicked.AddUniqueDynamic(this, &UPriestDeathWidget::RequestRestart);
    if (MainMenuButton) MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPriestDeathWidget::RequestMainMenu);
    ShowStatus(FText::GetEmpty());
}
void UPriestDeathWidget::SetBusy(bool bBusy)
{
    if (RestartButton) RestartButton->SetIsEnabled(!bBusy);
    if (MainMenuButton) MainMenuButton->SetIsEnabled(!bBusy);
}
void UPriestDeathWidget::ShowStatus(const FText& Message)
{
    if (StatusText) StatusText->SetText(Message);
}
void UPriestDeathWidget::FocusRestart()
{
    if (RestartButton && GetOwningPlayer()) RestartButton->SetUserFocus(GetOwningPlayer());
}
void UPriestDeathWidget::RequestRestart() { SendRequest(true); }
void UPriestDeathWidget::RequestMainMenu() { SendRequest(false); }
void UPriestDeathWidget::SendRequest(bool bRestart)
{
    TStrongObjectPtr<UPriestDeathRequest> Request(NewObject<UPriestDeathRequest>());
    Request->bRestart = bRestart;
    InvokeViewEvent(EViewEventType::ButtonClicked, Request.Get());
}
FDelegateHandle UPriestDeathWidget::AddListener(UMvcControl* Control) { return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent); }
void UPriestDeathWidget::RemoveListener(FDelegateHandle Handle) { Listener.Remove(Handle); }
void UPriestDeathWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter) { Listener.Broadcast(this, EventType, Parameter); }

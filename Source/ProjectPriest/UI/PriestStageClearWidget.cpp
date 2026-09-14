#include "PriestStageClearWidget.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "MvcControl.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/StrongObjectPtr.h"
bool UPriestStageClearWidget::Initialize()
{
    if (bBindingsReady)
    {
        return true;
    }
    JASSERT_BOOL((Super::Initialize()), "%hs: Base widget initialization failed", __FUNCTION__);
    JASSERT_BOOL((IsValid(MainMenuButton)), "%hs: Missing required MainMenuButton binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(ClearTimeText)), "%hs: Missing required ClearTimeText binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(StatusText)), "%hs: Missing required StatusText binding", __FUNCTION__);
    bBindingsReady = true;
    SetIsFocusable(true);
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPriestStageClearWidget::RequestMainMenu);
    ShowStatus(FText::GetEmpty());
    return true;
}
void UPriestStageClearWidget::SetBusy(bool bBusy)
{
    MainMenuButton->SetIsEnabled(!bBusy);
}
void UPriestStageClearWidget::ShowStatus(const FText& Message)
{
    StatusText->SetText(Message);
}
void UPriestStageClearWidget::FocusMainMenu()
{
    if (GetOwningPlayer())
    {
        MainMenuButton->SetUserFocus(GetOwningPlayer());
    }
}
void UPriestStageClearWidget::RequestMainMenu()
{
    TStrongObjectPtr<UPriestStageClearRequest> Request(NewObject<UPriestStageClearRequest>());
    InvokeViewEvent(EViewEventType::ButtonClicked, Request.Get());
}
FDelegateHandle UPriestStageClearWidget::AddListener(UMvcControl* Control)
{
    return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent);
}
void UPriestStageClearWidget::RemoveListener(FDelegateHandle Handle)
{
    Listener.Remove(Handle);
}
void UPriestStageClearWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter)
{
    Listener.Broadcast(this, EventType, Parameter);
}

FText UPriestStageClearWidget::FormatClearTime(float Seconds)
{
    const int32 Total = FMath::IsFinite(Seconds) ? static_cast<int32>(FMath::Clamp(static_cast<double>(Seconds), 0.0, 2147483647.0)) : 0;
    return FText::FromString(FString::Printf(TEXT("Clear time: %02d:%02d:%02d"), Total / 3600, (Total / 60) % 60, Total % 60));
}
void UPriestStageClearWidget::SetClearTime(float Seconds)
{
    ClearTimeText->SetText(FormatClearTime(Seconds));
}

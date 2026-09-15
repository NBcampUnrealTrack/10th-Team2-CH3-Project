#include "PriestStageClearWidget.h"
#include "MvcControl.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/StrongObjectPtr.h"
void UPriestStageClearWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (!IsValid(MainMenuButton))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Missing required MainMenuButton binding. Check the Widget Blueprint."), __FUNCTION__, *GetNameSafe(this));
    }
    if (!IsValid(ClearTimeText))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Missing required ClearTimeText binding. Check the Widget Blueprint."), __FUNCTION__, *GetNameSafe(this));
    }
    if (!IsValid(StatusText))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Missing required StatusText binding. Check the Widget Blueprint."), __FUNCTION__, *GetNameSafe(this));
    }
    SetIsFocusable(true);
    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPriestStageClearWidget::RequestMainMenu);
    }
    ShowStatus(FText::GetEmpty());
}
void UPriestStageClearWidget::SetBusy(bool bBusy)
{
    if (MainMenuButton)
    {
        MainMenuButton->SetIsEnabled(!bBusy);
    }
}
void UPriestStageClearWidget::ShowStatus(const FText& Message)
{
    if (StatusText)
    {
        StatusText->SetText(Message);
    }
}
void UPriestStageClearWidget::FocusMainMenu()
{
    if (MainMenuButton && GetOwningPlayer())
    {
        MainMenuButton->SetUserFocus(GetOwningPlayer());
    }
}
void UPriestStageClearWidget::RequestMainMenu()
{
    TStrongObjectPtr<UPriestStageClearRequest> Request(NewObject<UPriestStageClearRequest>());
    InvokeViewEvent(EViewEventType::ButtonClicked, Request.Get());
}
FDelegateHandle UPriestStageClearWidget::AddListener(UMvcControl* Control) { return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent); }
void UPriestStageClearWidget::RemoveListener(FDelegateHandle Handle) { Listener.Remove(Handle); }
void UPriestStageClearWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter) { Listener.Broadcast(this, EventType, Parameter); }

FText UPriestStageClearWidget::FormatClearTime(float Seconds)
{
    const int32 Total = FMath::IsFinite(Seconds) ? static_cast<int32>(FMath::Clamp(static_cast<double>(Seconds), 0.0, 2147483647.0)) : 0;
    return FText::FromString(FString::Printf(TEXT("Clear time: %02d:%02d:%02d"), Total / 3600, (Total / 60) % 60, Total % 60));
}
void UPriestStageClearWidget::SetClearTime(float Seconds)
{
    if (ClearTimeText)
    {
        ClearTimeText->SetText(FormatClearTime(Seconds));
    }
}

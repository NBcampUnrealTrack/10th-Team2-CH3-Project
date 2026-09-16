#include "PriestQuickSlotController.h"
#include "PriestQuickSlotModel.h"
#include "PriestQuickSlotWidget.h"
#include "PriestQuickSlotEventParameter.h"
#include "JUtility.h"

void UPriestQuickSlotController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestQuickSlotModel* Model = GetModel<UPriestQuickSlotModel>();
    UPriestQuickSlotWidget* View = GetView<UPriestQuickSlotWidget>();
    if (IsValid(Model) && InModel == Model && IsValid(View))
    {
        View->SetQuickSlotData(Model->GetData());
    }
}

void UPriestQuickSlotController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    UPriestQuickSlotModel* Model =
        GetModel<UPriestQuickSlotModel>();

    UPriestQuickSlotWidget* View =
        GetView<UPriestQuickSlotWidget>();

    // Controller 자체가 정상적으로 구성되지 않은 경우
    JASSERT_RETURN(
        IsValid(Model),
        ,
        "%hs: Model이 유효하지 않습니다.",
        __FUNCTION__
    );

    JASSERT_RETURN(
        IsValid(View),
        ,
        "%hs: View가 유효하지 않습니다.",
        __FUNCTION__
    );

    if (InView != View)
    {
        return;
    }

    if (EventType != EViewEventType::QuickSlotRequest)
    {
        return;
    }

    UPriestQuickSlotEventParameter* QuickSlotParameter =
        Cast<UPriestQuickSlotEventParameter>(Parameter);

    JASSERT_RETURN(
        IsValid(QuickSlotParameter),
        ,
        "%hs: QuickSlotRequest의 Parameter 타입이 잘못되었습니다.",
        __FUNCTION__
    );

    switch (QuickSlotParameter->Action)
    {
    case EPriestQuickSlotAction::Assign:
        QuickSlotParameter->bAccepted =
            Model->AssignItem(
                QuickSlotParameter->ItemId
            );
        break;

    case EPriestQuickSlotAction::Clear:
        QuickSlotParameter->bAccepted =
            Model->ClearItem();
        break;

    default:
        JASSERT(
            false,
            "%hs: 지원하지 않는 QuickSlot Action입니다.",
            __FUNCTION__
        );
        QuickSlotParameter->bAccepted = false;
        break;
    }
}

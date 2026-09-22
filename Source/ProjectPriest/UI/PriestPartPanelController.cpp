#include "PriestPartPanelController.h"
#include "PriestPartPanelModel.h"
#include "PriestPartPanelWidget.h"
#include "PriestPartSlotEventParameter.h"
#include "Engine/Engine.h"
#include "JUtility.h"

void UPriestPartPanelController::HandleViewEvent(
    IMvcView* InView,
    EViewEventType EventType,
    UEventParameterBase* Parameter
)
{
    if (EventType != EViewEventType::PartSlotRequest)
    {
        return;
    }

    UPriestPartPanelModel* Model = GetModel<UPriestPartPanelModel>();

    UPriestPartPanelWidget* View = GetView<UPriestPartPanelWidget>();

    JASSERT(IsValid(Model), "PartPanelModel이 없습니다.");

    JASSERT(IsValid(View), "PartPanelWidget이 없습니다.");

    JASSERT((InView == View), "연결된 View와 요청한 View가 다릅니다.");

    UPriestPartSlotEventParameter* PartParameter =
        Cast<UPriestPartSlotEventParameter>(Parameter);

    JASSERT(IsValid(PartParameter), "파츠 요청의 Parameter 타입이 잘못되었습니다.");

    Model->EquipPart(PartParameter->ItemId, PartParameter->SlotType);
}

void UPriestPartPanelController::HandleModelChanged(
    IMvcModel* InModel,
    uint8 PropertyName
)
{
    UPriestPartPanelModel* Model = GetModel<UPriestPartPanelModel>();

    UPriestPartPanelWidget* View = GetView<UPriestPartPanelWidget>();

    JASSERT(IsValid(Model), "PartPanelModel이 없습니다.");

    JASSERT(IsValid(View), "PartPanelWidget이 없습니다.");

    JASSERT((InModel == Model), "연결된 Model과 변경을 알린 Model이 다릅니다.");

    View->UpdatePartSlots(Model->GetPart(EPartSlot::Barrel), Model->GetPart(EPartSlot::Magazine));
}

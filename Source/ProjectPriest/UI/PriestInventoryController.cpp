#include "PriestInventoryController.h"
#include "PriestInventoryModel.h"
#include "PriestMainMenuWidget.h"
#include "PriestInventoryEventParameter.h"
#include "JUtility.h"
#include "Engine/Engine.h"

void UPriestInventoryController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestInventoryModel* Model = GetModel<UPriestInventoryModel>();

    UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();

    JASSERT(IsValid(Model), "%hs: InventoryModel이 유효하지 않습니다.", __FUNCTION__);

    JASSERT(IsValid(View), "%hs: Inventory View가 유효하지 않습니다.", __FUNCTION__);

    JASSERT(InModel == Model, "%hs: 연결된 Model과 이벤트 발생 Model이 다릅니다.", __FUNCTION__);

    View->SetInventoryData(Model->GetData());
}

void UPriestInventoryController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
    UPriestInventoryModel* Model = GetModel<UPriestInventoryModel>();

    UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();

    JASSERT(IsValid(Model), "%hs: InventoryModel이 유효하지 않습니다.", __FUNCTION__);

    JASSERT(IsValid(View), "%hs: Inventory View가 유효하지 않습니다.", __FUNCTION__);

    JASSERT(InView == View, "%hs: 연결된 View와 이벤트 발생 View가 다릅니다.", __FUNCTION__);

    if (EventType != EViewEventType::InventoryRequest)
    {
        return;
    }

    UPriestInventoryEventParameter* Request = Cast<UPriestInventoryEventParameter>(Parameter);

    JASSERT(IsValid(Request), "%hs: InventoryRequest Parameter 타입이 잘못되었습니다.", __FUNCTION__);

    switch (Request->Action)
    {
    case EPriestInventoryAction::SelectCategory:
        Request->bAccepted = Model->SetCategory(Request->Category);
        break;

    case EPriestInventoryAction::EquipWeapon:
        Request->bAccepted = Model->EquipWeapon(Request->ItemId);
        break;

    default:
        Request->bAccepted = false;
        break;
    }
}

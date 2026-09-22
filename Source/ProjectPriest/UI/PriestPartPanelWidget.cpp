#include "PriestPartPanelWidget.h"

#include "PriestPartSlotEventParameter.h"
#include "PriestPartPanelModel.h"
#include "PriestPartPanelController.h"
#include "Attachment/PartSubsystem.h"
#include "PriestInventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h"
#include "JUtility.h"
#include "MvcControl.h"
#include "MvcUtility.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_VIEW_DEFAULT_ADDLISTENER(UPriestPartPanelWidget)
IMPLEMENT_VIEW_DEFAULT_REMOVELISTENER(UPriestPartPanelWidget)
IMPLEMENT_VIEW_DEFAULT_INVOKE_VIEW_EVENT(UPriestPartPanelWidget)

void UPriestPartPanelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Model = NewObject<UPriestPartPanelModel>(this);
    MvcControl = NewObject<UPriestPartPanelController>(this);
}

void UPriestPartPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    JASSERT(IsValid(Model), "모델이 없습니다.");
    JASSERT(IsValid(MvcControl), "컨트롤러가 없습니다.");
    MvcControl->Disconnect();

    UGameInstance* GameInstance = GetGameInstance();
    JASSERT(IsValid(GameInstance), "GameInstance가 없습니다.");
    JASSERT(IsValid(PartDefinitionTable), "PartDefinitionTable을 지정하세요.");
    JASSERT((PartDefinitionTable->GetRowStruct() == FPartData::StaticStruct()), "FPartData 행 구조의 테이블이 필요합니다.");

    UPartSubsystem* Parts = GameInstance->GetSubsystem<UPartSubsystem>();
    UPriestInventorySubsystem* Inventory = GameInstance->GetSubsystem<UPriestInventorySubsystem>();
    JASSERT(IsValid(Parts), "PartSubsystem이 없습니다.");
    JASSERT(IsValid(Inventory), "InventorySubsystem이 없습니다.");

    Model->Initialize(Parts, Inventory, PartDefinitionTable, WeaponId);
    MvcControl->SetModel(Model);
    MvcControl->SetView(this);
    MvcControl->HandleModelChanged(Model, 0);
}

void UPriestPartPanelWidget::NativeDestruct()
{
    if (IsValid(MvcControl))
    {
        MvcControl->Disconnect();
    }
    MvcControl = nullptr;
    Model = nullptr;

    Super::NativeDestruct();
}

void UPriestPartPanelWidget::SetWeaponId(FName InWeaponId)
{
    WeaponId = InWeaponId;
    Model->SetWeaponId(InWeaponId);
}

void UPriestPartPanelWidget::RequestEquipPart(FName ItemId, EPartSlot SlotType)
{
    if (ItemId.IsNone() || (SlotType != EPartSlot::Barrel && SlotType != EPartSlot::Magazine))
    {
        return;
    }

    TStrongObjectPtr<UPriestPartSlotEventParameter> Parameter(
        NewObject<UPriestPartSlotEventParameter>()
    );

    Parameter->ItemId = ItemId;
    Parameter->SlotType = SlotType;

    InvokeViewEvent(EViewEventType::PartSlotRequest, Parameter.Get());
}

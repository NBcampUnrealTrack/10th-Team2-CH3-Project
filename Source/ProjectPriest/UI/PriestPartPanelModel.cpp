#include "PriestPartPanelModel.h"
#include "Attachment/PartSubsystem.h"
#include "PriestInventorySubsystem.h"
#include "PriestItemTypes.h"
#include "Engine/DataTable.h"
#include "MvcControl.h"
#include "Engine/Engine.h"
#include "JUtility.h"

void UPriestPartPanelModel::Initialize(
    UPartSubsystem* InParts,
    UPriestInventorySubsystem* InInventory,
    UDataTable* InPartTable,
    FName InWeaponId
)
{
    JASSERT(IsValid(InParts), "PartSubsystem이 없습니다.");
    JASSERT(IsValid(InInventory), "InventorySubsystem이 없습니다.");
    JASSERT(IsValid(InPartTable), "파츠 정의 테이블이 없습니다.");
    JASSERT((InPartTable->GetRowStruct() == FPartData::StaticStruct()),
        "FPartData 행 구조의 테이블이 필요합니다.");

    Parts = InParts;
    Inventory = InInventory;
    PartTable = InPartTable;
    WeaponId = InWeaponId;
}

void UPriestPartPanelModel::SetWeaponId(FName InWeaponId)
{
    WeaponId = InWeaponId;
    InvokePropertyChanged(0);
}

bool UPriestPartPanelModel::EquipPart(FName ItemId, EPartSlot SlotType)
{
    JASSERT_BOOL(IsValid(Parts), "PartSubsystem이 없습니다.");
    JASSERT_BOOL(IsValid(Inventory), "InventorySubsystem이 없습니다.");
    JASSERT_BOOL(IsValid(PartTable), "파츠 정의 테이블이 없습니다.");
    JASSERT_BOOL((PartTable->GetRowStruct() == FPartData::StaticStruct()),
        "FPartData 행 구조의 테이블이 필요합니다.");
    JASSERT_BOOL((SlotType == EPartSlot::Barrel || SlotType == EPartSlot::Magazine),
        "잘못된 파츠 슬롯입니다.");

    if (WeaponId.IsNone() || ItemId.IsNone())
    {
        return false;
    }

    FPriestItemData ItemData;
    if (!Inventory->TryGetItemData(ItemId, ItemData)
        || ItemData.Category != EItemCategory::Part
        || Inventory->GetQuantity(ItemId) <= 0)
    {
        return false;
    }

    const FPartData* PartRow = PartTable->FindRow<FPartData>(ItemId, TEXT("PartPanelModel"), false);
    JASSERT_BOOL((PartRow != nullptr),
        "보유 파츠의 정의 행이 없습니다.");
    JASSERT_BOOL((PartRow->SlotType == EPartSlot::Barrel || PartRow->SlotType == EPartSlot::Magazine),
        "파츠 정의의 슬롯 종류가 잘못되었습니다.");

    if (PartRow->SlotType != SlotType)
    {
        return false;
    }

    FPartData Part = *PartRow;
    Part.Name = ItemId;
    Parts->SetPart(WeaponId, Part);

    InvokePropertyChanged(0);
    return true;
}

FPartData UPriestPartPanelModel::GetPart(EPartSlot SlotType) const
{
    JASSERT_RETURN(IsValid(Parts), FPartData{}, "PartSubsystem이 없습니다.");
    JASSERT_RETURN((SlotType == EPartSlot::Barrel || SlotType == EPartSlot::Magazine), FPartData{},
        "잘못된 파츠 슬롯입니다.");

    if (WeaponId.IsNone())
    {
        return FPartData{};
    }

    return Parts->GetEquippedPart(WeaponId, SlotType);
}

FDelegateHandle UPriestPartPanelModel::AddListener(UMvcControl* Control)
{
    JASSERT_RETURN(IsValid(Control), FDelegateHandle{}, "Controller가 없습니다.");
    return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}

void UPriestPartPanelModel::RemoveListener(FDelegateHandle Handle)
{
    Changed.Remove(Handle);
}

void UPriestPartPanelModel::InvokePropertyChanged(uint8 PropertyName)
{
    Changed.Broadcast(this, PropertyName);
}

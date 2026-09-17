#include "PriestQuickSlotModel.h"
#include "PriestInventorySubsystem.h"
#include "MvcControl.h"
#include "Engine/Engine.h"
#include "JUtility.h"

void UPriestQuickSlotModel::Initialize(UPriestInventorySubsystem* InInventory, int32 InSlotIndex)
{
    Disconnect();
    Data = FPriestQuickSlotData{};
    JASSERT(IsValid(InInventory), "%hs: InventorySubsystem이 없습니다.", __FUNCTION__);
    JASSERT(0 <= InSlotIndex && InSlotIndex <= 1, "%hs: 잘못된 슬롯 인덱스 %d", __FUNCTION__, InSlotIndex);
    Inventory = InInventory;
    Data.SlotIndex = InSlotIndex;
    Inventory->OnInventoryChanged.AddUniqueDynamic(this, &UPriestQuickSlotModel::Refresh);
    Inventory->OnQuickSlotsChanged.AddUniqueDynamic(this, &UPriestQuickSlotModel::Refresh);
    Refresh();
}

void UPriestQuickSlotModel::Disconnect()
{
    if (IsValid(Inventory))
    {
        Inventory->OnInventoryChanged.RemoveDynamic(this, &UPriestQuickSlotModel::Refresh);
        Inventory->OnQuickSlotsChanged.RemoveDynamic(this, &UPriestQuickSlotModel::Refresh);
    }
    Inventory = nullptr;
}

bool UPriestQuickSlotModel::AssignItem(FName ItemId)
{
    JASSERT_BOOL(IsValid(Inventory), "%hs [%s]: InventorySubsystem이 없습니다.", __FUNCTION__, *GetNameSafe(this));

    JASSERT_BOOL(!ItemId.IsNone(), "%hs [%s]: ItemId가 유효하지 않습니다.", __FUNCTION__, *GetNameSafe(this));

    return Inventory->AssignQuickSlot(Data.SlotIndex, ItemId);
}

bool UPriestQuickSlotModel::ClearItem()
{
    JASSERT_BOOL(IsValid(Inventory), "%hs [%s]: InventorySubsystem이 없습니다.", __FUNCTION__, *GetNameSafe(this));

    return Inventory->ClearQuickSlot(Data.SlotIndex);
}

void UPriestQuickSlotModel::BeginDestroy()
{
    Disconnect();
    Super::BeginDestroy();
}

const FPriestQuickSlotData& UPriestQuickSlotModel::GetData() const
{
    return Data;
}

void UPriestQuickSlotModel::Refresh()
{
    JASSERT(IsValid(Inventory), "%hs: InventorySubsystem이 없습니다.", __FUNCTION__);
    const FName ItemId = Inventory->GetQuickSlotItemId(Data.SlotIndex);
    if (ItemId != Data.ItemId)
    {
        Data.DisplayName = FText::GetEmpty();
    }
    Data.ItemId = ItemId;
    Data.bAssigned = !ItemId.IsNone();
    Data.Quantity = Data.bAssigned ? Inventory->GetQuantity(ItemId) : 0;
    if (Data.bAssigned)
    {
        for (const FPriestOwnedItem& Item : Inventory->GetOwnedItems())
        {
            if (Item.ItemId == ItemId)
            {
                Data.DisplayName = Item.DisplayName;
                break;
            }
        }
        if (Data.DisplayName.IsEmpty()) Data.DisplayName = FText::FromName(ItemId);
    }
    InvokePropertyChanged(0);
}

FDelegateHandle UPriestQuickSlotModel::AddListener(UMvcControl* Control)
{
    return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}
void UPriestQuickSlotModel::RemoveListener(FDelegateHandle Handle) 
{ 
    Changed.Remove(Handle); 
}
void UPriestQuickSlotModel::InvokePropertyChanged(uint8 PropertyName) 
{ 
    Changed.Broadcast(this, PropertyName); 
}

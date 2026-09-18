#include "PriestInventoryModel.h"
#include "PriestInventorySubsystem.h"
#include "MvcControl.h"
#include "JUtility.h"
#include "Engine/Engine.h"

void UPriestInventoryModel::Initialize(UPriestInventorySubsystem* InInventory)
{
    Disconnect();

    JASSERT(IsValid(InInventory), "%hs: InventorySubsystem이 유효하지 않습니다.", __FUNCTION__);

    Inventory = InInventory;

    Inventory->OnInventoryChanged.AddUniqueDynamic(this, &UPriestInventoryModel::Refresh);

    Refresh();
}

void UPriestInventoryModel::Disconnect()
{
    if (IsValid(Inventory))
    {
        Inventory->OnInventoryChanged.RemoveDynamic(this, &UPriestInventoryModel::Refresh);
    }

    Inventory = nullptr;
}

void UPriestInventoryModel::BeginDestroy()
{
    Disconnect();
    Super::BeginDestroy();
}

bool UPriestInventoryModel::SetCategory(EItemCategory InCategory)
{
    if (!IsValid(Inventory))
    {
        return false;
    }

    switch (InCategory)
    {
        case EItemCategory::Weapon:
        case EItemCategory::Ammo:
        case EItemCategory::Consumable:
        case EItemCategory::Material:
            break;
        default:
            return false;
    }

    if (Data.SelectedCategory == InCategory)
    {
        return true;
    }

    Data.SelectedCategory = InCategory;

    Refresh();
    return true;
}

void UPriestInventoryModel::Refresh()
{
    JASSERT(IsValid(Inventory), "%hs: InventorySubsystem이 유효하지 않습니다.", __FUNCTION__);

    Data.Items.Reset();

    const TArray<FPriestOwnedItem> Items =
        Inventory->GetItemsByCategory(Data.SelectedCategory);

    for (const FPriestOwnedItem& Item : Items)
    {
        FPriestInventorySlotData SlotData;

        SlotData.ItemId = Item.ItemId;
        SlotData.DisplayName = Item.DisplayName;
        SlotData.Quantity = Item.Quantity;

        SlotData.Category = Data.SelectedCategory;

        FPotionData PotionData;

        SlotData.bQuickSlotCompatible =
            Inventory->TryGetPotionData(
                Item.ItemId,
                PotionData
            );

        Data.Items.Add(MoveTemp(SlotData));
    }

    InvokePropertyChanged(0);
}

FDelegateHandle UPriestInventoryModel::AddListener(UMvcControl* Control)
{
    return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}

void UPriestInventoryModel::RemoveListener(FDelegateHandle Handle)
{
    Changed.Remove(Handle);
}

void UPriestInventoryModel::InvokePropertyChanged(uint8 PropertyName)
{
    Changed.Broadcast(this, PropertyName);
}

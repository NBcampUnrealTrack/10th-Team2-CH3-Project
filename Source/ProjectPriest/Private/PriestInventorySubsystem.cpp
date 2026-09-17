#include "PriestInventorySubsystem.h"

bool UPriestInventorySubsystem::AddItem(FName ItemId, FText DisplayName, int32 Quantity)
{
    if (ItemId.IsNone() || DisplayName.IsEmpty() || Quantity <= 0)
    {
        return false;
    }
    for (FPriestOwnedItem& Item : OwnedItems)
    {
        if (Item.ItemId == ItemId)
        {
            if (Item.Quantity > MAX_int32 - Quantity)
            {
                return false;
            }
            Item.Quantity += Quantity;
            OnInventoryChanged.Broadcast();
            return true;
        }
    }
    FPriestOwnedItem Item;
    Item.ItemId = ItemId;
    Item.DisplayName = DisplayName;
    Item.Quantity = Quantity;
    OwnedItems.Add(Item);
    OnInventoryChanged.Broadcast();
    return true;
}

int32 UPriestInventorySubsystem::GetQuantity(FName ItemId) const
{
    const FPriestOwnedItem* Item = OwnedItems.FindByPredicate(
        [ItemId](const FPriestOwnedItem& Entry) { return Entry.ItemId == ItemId; });
    return Item ? Item->Quantity : 0;
}

bool UPriestInventorySubsystem::RemoveItem(FName ItemId, int32 Quantity)
{
    const int32 Index = OwnedItems.IndexOfByPredicate(
        [ItemId](const FPriestOwnedItem& Entry) { return Entry.ItemId == ItemId; });
    if (Quantity <= 0 || Index == INDEX_NONE || OwnedItems[Index].Quantity < Quantity)
    {
        return false;
    }
    OwnedItems[Index].Quantity -= Quantity;
    if (OwnedItems[Index].Quantity == 0)
    {
        OwnedItems.RemoveAt(Index);
    }
    OnInventoryChanged.Broadcast();
    return true;
}

void UPriestInventorySubsystem::GrantPreviewItemsOnce()
{
    if (bPreviewItemsGranted)
    {
        return;
    }
    bPreviewItemsGranted = true;
    AddItem(TEXT("Potion.Health"), NSLOCTEXT("PriestInventory", "HealthPotion", "회복 포션"), 5);
    AddItem(TEXT("Potion.Health.Large"), NSLOCTEXT("PriestInventory", "LargeHealthPotion", "상급 회복 포션"), 2);
    AddItem(TEXT("Material.Herb"), NSLOCTEXT("PriestInventory", "Herb", "약초"), 12);
    AddItem(TEXT("Material.Essence"), NSLOCTEXT("PriestInventory", "Essence", "정수"), 4);
}

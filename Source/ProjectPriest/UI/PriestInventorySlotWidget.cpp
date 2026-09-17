#include "PriestInventorySlotWidget.h"
#include "Components/TextBlock.h"

void UPriestInventorySlotWidget::SetItem(const FPriestOwnedItem& InItem)
{
    Item = InItem;
    RefreshItem();
}

void UPriestInventorySlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    RefreshItem();
}

void UPriestInventorySlotWidget::RefreshItem()
{
    if (ItemNameText)
    {
        ItemNameText->SetText(Item.DisplayName);
    }
    if (QuantityText)
    {
        QuantityText->SetText(FText::Format(NSLOCTEXT("PriestInventory", "Quantity", "× {0}"), FText::AsNumber(Item.Quantity)));
    }
    OnItemChanged(Item);
}

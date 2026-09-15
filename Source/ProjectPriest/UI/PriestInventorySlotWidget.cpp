#include "PriestInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"
#include "JUtility.h"

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
    if (!IsDesignTime())
    {
        JASSERT(IsValid(ItemNameText), "%hs [%s]: ItemNameText 바인딩을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
        JASSERT(IsValid(QuantityText), "%hs [%s]: QuantityText 바인딩을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
    }
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

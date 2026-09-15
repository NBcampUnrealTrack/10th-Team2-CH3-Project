#include "PriestQuickSlotWidget.h"
#include "PriestInventorySubsystem.h"
#include "Components/TextBlock.h"
#include "Engine/GameInstance.h"

void UPriestQuickSlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (IsDesignTime())
    {
        if (KeyText) KeyText->SetText(FText::AsNumber(SlotIndex + 1));
        if (ItemNameText) ItemNameText->SetText(NSLOCTEXT("PriestQuickSlot", "PreviewPotion", "회복 포션"));
        if (QuantityText) QuantityText->SetText(FText::FromString(TEXT("× 5")));
        SetRenderOpacity(1.0f);
    }
}

void UPriestQuickSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    DisconnectInventory();
    if (UGameInstance* Instance = GetGameInstance())
    {
        Inventory = Instance->GetSubsystem<UPriestInventorySubsystem>();
    }
    if (IsValid(Inventory))
    {
        Inventory->OnInventoryChanged.AddUniqueDynamic(this, &UPriestQuickSlotWidget::RefreshQuickSlot);
        Inventory->OnQuickSlotsChanged.AddUniqueDynamic(this, &UPriestQuickSlotWidget::RefreshQuickSlot);
    }
    RefreshQuickSlot();
}

void UPriestQuickSlotWidget::NativeDestruct()
{
    DisconnectInventory();
    Super::NativeDestruct();
}

void UPriestQuickSlotWidget::DisconnectInventory()
{
    if (IsValid(Inventory))
    {
        Inventory->OnInventoryChanged.RemoveDynamic(this, &UPriestQuickSlotWidget::RefreshQuickSlot);
        Inventory->OnQuickSlotsChanged.RemoveDynamic(this, &UPriestQuickSlotWidget::RefreshQuickSlot);
    }
    Inventory = nullptr;
}

void UPriestQuickSlotWidget::SetSlotIndex(int32 InSlotIndex)
{
    if (InSlotIndex < 0 || InSlotIndex > 1)
    {
        return;
    }
    SlotIndex = InSlotIndex;
    RefreshQuickSlot();
}

void UPriestQuickSlotWidget::RefreshQuickSlot()
{
    if (!KeyText || !ItemNameText || !QuantityText)
    {
        return;
    }

    KeyText->SetText(FText::AsNumber(SlotIndex + 1));
    const FName ItemId = IsValid(Inventory)
        ? Inventory->GetQuickSlotItemId(SlotIndex) : NAME_None;

    if (ItemId != LastItemId)
    {
        LastItemId = ItemId;
        LastItemName = FText::GetEmpty();
    }
    if (ItemId.IsNone())
    {
        ItemNameText->SetText(NSLOCTEXT("PriestQuickSlot", "Unassigned", "미등록"));
        QuantityText->SetText(FText::GetEmpty());
        SetRenderOpacity(EmptyOpacity);
        return;
    }

    const int32 Quantity = Inventory->GetQuantity(ItemId);
    for (const FPriestOwnedItem& Item : Inventory->GetOwnedItems())
    {
        if (Item.ItemId == ItemId)
        {
            LastItemName = Item.DisplayName;
            break;
        }
    }
    // Keep the last observed name when the stack reaches zero. A fresh widget
    // falls back to the ID until a shared item-definition lookup is introduced.
    ItemNameText->SetText(LastItemName.IsEmpty() ? FText::FromName(ItemId) : LastItemName);
    QuantityText->SetText(FText::Format(NSLOCTEXT("PriestQuickSlot", "Quantity", "× {0}"), FText::AsNumber(Quantity)));
    SetRenderOpacity(Quantity > 0 ? 1.0f : EmptyOpacity);
}

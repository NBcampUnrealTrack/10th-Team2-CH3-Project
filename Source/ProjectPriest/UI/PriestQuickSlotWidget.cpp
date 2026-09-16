#include "PriestQuickSlotWidget.h"
#include "PriestInventorySubsystem.h"
#include "PriestInventoryDragDropOperation.h"
#include "Components/TextBlock.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "JUtility.h"

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
    UGameInstance* Instance = GetGameInstance();
    JASSERT(IsValid(Instance), "%hs [%s]: GameInstance가 없습니다.", __FUNCTION__, *GetNameSafe(this));
    Inventory = Instance->GetSubsystem<UPriestInventorySubsystem>();
    JASSERT(IsValid(Inventory), "%hs [%s]: InventorySubsystem 연결에 실패했습니다.", __FUNCTION__, *GetNameSafe(this));
    JASSERT(0 <= SlotIndex && SlotIndex <= 1, "%hs [%s]: SlotIndex=%d, 허용 범위는 0~1입니다.", __FUNCTION__, *GetNameSafe(this), SlotIndex);
    Inventory->OnInventoryChanged.AddUniqueDynamic(this, &UPriestQuickSlotWidget::RefreshQuickSlot);
    Inventory->OnQuickSlotsChanged.AddUniqueDynamic(this, &UPriestQuickSlotWidget::RefreshQuickSlot);
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
    JASSERT(0 <= InSlotIndex && InSlotIndex <= 1, "%hs [%s]: SlotIndex=%d, 허용 범위는 0~1입니다.", __FUNCTION__, *GetNameSafe(this), InSlotIndex);
    SlotIndex = InSlotIndex;
    RefreshQuickSlot();
}

void UPriestQuickSlotWidget::RefreshQuickSlot()
{
    JASSERT(IsValid(KeyText), "%hs [%s]: KeyText 바인딩이 유효하지 않습니다. WBP의 Text Block 이름과 타입을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
    JASSERT(IsValid(ItemNameText), "%hs [%s]: ItemNameText 바인딩이 유효하지 않습니다. WBP의 Text Block 이름과 타입을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
    JASSERT(IsValid(QuantityText), "%hs [%s]: QuantityText 바인딩이 유효하지 않습니다. WBP의 Text Block 이름과 타입을 확인하세요.", __FUNCTION__, *GetNameSafe(this));

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

bool UPriestQuickSlotWidget::NativeOnDrop(
    const FGeometry& InGeometry,
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation
)
{
    UPriestInventoryDragDropOperation* InventoryOperation =
        Cast<UPriestInventoryDragDropOperation>(InOperation);

    if (!IsValid(InventoryOperation))
    {
        return false;
    }

    JASSERT_BOOL(
        (IsValid(Inventory)),
        "%hs [%s]: InventorySubsystem이 없습니다.",
        __FUNCTION__,
        *GetNameSafe(this)
    );

    return Inventory->AssignQuickSlot(
        SlotIndex,
        InventoryOperation->ItemId
    );
}

FReply UPriestQuickSlotWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent
)
{
    if (InMouseEvent.GetEffectingButton()
        != EKeys::RightMouseButton)
    {
        return Super::NativeOnMouseButtonDown(
            InGeometry,
            InMouseEvent
        );
    }

    JASSERT_RETURN(
        (IsValid(Inventory)),
        FReply::Handled(),
        "%hs [%s]: InventorySubsystem이 없습니다.",
        __FUNCTION__,
        *GetNameSafe(this)
    );

    Inventory->ClearQuickSlot(SlotIndex);

    return FReply::Handled();
}

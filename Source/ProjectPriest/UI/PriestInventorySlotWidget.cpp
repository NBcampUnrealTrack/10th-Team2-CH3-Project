#include "PriestInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "PriestInventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Engine.h"
#include "InputCoreTypes.h"
#include "JUtility.h"

void UPriestInventorySlotWidget::SetItem(const FPriestInventorySlotData& InItem)
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

FReply UPriestInventorySlotWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent
)
{
    if (InMouseEvent.GetEffectingButton()
        == EKeys::LeftMouseButton
        && !Item.ItemId.IsNone()
        && Item.Quantity > 0)
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(
            InMouseEvent,
            this,
            EKeys::LeftMouseButton
        ).NativeReply;
    }

    return Super::NativeOnMouseButtonDown(
        InGeometry,
        InMouseEvent
    );
}

void UPriestInventorySlotWidget::NativeOnDragDetected(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation
)
{
    OutOperation = nullptr;

    const bool bIsEmptySlot =
        Item.ItemId.IsNone()
        || Item.Quantity <= 0;

    if (bIsEmptySlot)
    {
        return;
    }

    if (!Item.bQuickSlotCompatible)
    {
        return;
    }

    UPriestInventorySlotWidget* DragVisual =
        CreateWidget<UPriestInventorySlotWidget>(
            GetOwningPlayer(),
            GetClass()
        );

    JASSERT(IsValid(DragVisual), "%hs: 드래그 표시 위젯 생성에 실패했습니다.", __FUNCTION__);

    DragVisual->SetItem(Item);

    DragVisual->SetVisibility(
        ESlateVisibility::HitTestInvisible
    );

    DragVisual->SetRenderOpacity(0.75f);

    UPriestInventoryDragDropOperation* Operation =
        Cast<UPriestInventoryDragDropOperation>(
            UWidgetBlueprintLibrary::CreateDragDropOperation(
                UPriestInventoryDragDropOperation::StaticClass()
            )
        );

    JASSERT(IsValid(Operation), "%hs: 드래그 작업 생성에 실패했습니다.", __FUNCTION__);

    Operation->ItemId = Item.ItemId;
    Operation->DefaultDragVisual = DragVisual;
    Operation->Pivot = EDragPivot::MouseDown;

    OutOperation = Operation;
}

#include "PriestQuickSlotWidget.h"
#include "PriestInventoryDragDropOperation.h"
#include "PriestQuickSlotEventParameter.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"
#include "JUtility.h"
#include "PriestUIManager.h"
#include "Engine/LocalPlayer.h"
#include "InputCoreTypes.h"

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

    if (UIManager.IsValid())
    {
        UIManager->DisconnectQuickSlot(this);
    }

    ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();

    JASSERT_RETURN(
        IsValid(LocalPlayer),
        ,
        "%hs: Owning LocalPlayer가 없습니다.",
        __FUNCTION__
    );

    UIManager =
        LocalPlayer->GetSubsystem<UPriestUIManager>();

    JASSERT_RETURN(
        UIManager.IsValid(),
        ,
        "%hs: UIManager가 없습니다.",
        __FUNCTION__
    );

    UIManager->ConnectQuickSlot(this, SlotIndex);
}

void UPriestQuickSlotWidget::NativeDestruct()
{
    if (UIManager.IsValid()) UIManager->DisconnectQuickSlot(this);
    UIManager.Reset();
    Super::NativeDestruct();
}

void UPriestQuickSlotWidget::SetSlotIndex(int32 InSlotIndex)
{
    JASSERT(InSlotIndex >= 0 && InSlotIndex <= 1, "%hs: 잘못된 슬롯 인덱스 %d", __FUNCTION__, InSlotIndex);
    SlotIndex = InSlotIndex;
    if (UIManager.IsValid()) UIManager->ConnectQuickSlot(this, SlotIndex);
}

void UPriestQuickSlotWidget::SetQuickSlotData(const FPriestQuickSlotData& InData)
{
    ViewData = InData;
    RefreshDisplay();
}

void UPriestQuickSlotWidget::RefreshDisplay()
{
    JASSERT(IsValid(KeyText), "%hs [%s]: KeyText 바인딩을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
    JASSERT(IsValid(ItemNameText), "%hs [%s]: ItemNameText 바인딩을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
    JASSERT(IsValid(QuantityText), "%hs [%s]: QuantityText 바인딩을 확인하세요.", __FUNCTION__, *GetNameSafe(this));
    KeyText->SetText(FText::AsNumber(ViewData.SlotIndex + 1));
    if (!ViewData.bAssigned)
    {
        ItemNameText->SetText(NSLOCTEXT("PriestQuickSlot", "Unassigned", "미등록"));
        QuantityText->SetText(FText::GetEmpty());
        SetRenderOpacity(EmptyOpacity);
        return;
    }
    ItemNameText->SetText(ViewData.DisplayName);
    QuantityText->SetText(FText::Format(NSLOCTEXT("PriestQuickSlot", "Quantity", "× {0}"), FText::AsNumber(ViewData.Quantity)));
    SetRenderOpacity(ViewData.Quantity > 0 ? 1.0f : EmptyOpacity);
}

FDelegateHandle UPriestQuickSlotWidget::AddListener(UMvcControl* Control)
{
    return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent);
}
void UPriestQuickSlotWidget::RemoveListener(FDelegateHandle Handle) { Listener.Remove(Handle); }
void UPriestQuickSlotWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter)
{
    Listener.Broadcast(this, EventType, Parameter);
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

    TStrongObjectPtr<UPriestQuickSlotEventParameter> Parameter(
        NewObject<UPriestQuickSlotEventParameter>()
    );

    Parameter->Action = EPriestQuickSlotAction::Assign;
    Parameter->ItemId = InventoryOperation->ItemId;

    InvokeViewEvent(
        EViewEventType::QuickSlotRequest,
        Parameter.Get()
    );

    return Parameter->bAccepted;
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

    TStrongObjectPtr<UPriestQuickSlotEventParameter> Parameter(
        NewObject<UPriestQuickSlotEventParameter>()
    );

    Parameter->Action = EPriestQuickSlotAction::Clear;

    InvokeViewEvent(
        EViewEventType::QuickSlotRequest,
        Parameter.Get()
    );

    return FReply::Handled();
}

#include "PriestInventorySubsystem.h"
#include "Engine/Engine.h"
#include "PriestInventorySettings.h"
#include "Engine/DataTable.h"
#include "UObject/UObjectGlobals.h"
#include "JUtility.h"

bool UPriestInventorySubsystem::AddItem(FName ItemId, FText DisplayName, int32 Quantity)
{
    if (ItemId.IsNone() || DisplayName.IsEmpty() || Quantity <= 0)
    {
        JError("%hs: 잘못된 추가 요청. ItemId=%s, Name=%s, Quantity=%d", __FUNCTION__, *ItemId.ToString(), *DisplayName.ToString(), Quantity);
        return false;
    }
    for (FPriestOwnedItem& Item : OwnedItems)
    {
        if (Item.ItemId == ItemId)
        {
            if (Item.Quantity > MAX_int32 - Quantity)
            {
                JError("%hs: 수량 범위 초과. ItemId=%s, Current=%d, Add=%d", __FUNCTION__, *ItemId.ToString(), Item.Quantity, Quantity);
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
    JASSERT_BOOL((!ItemId.IsNone() && Quantity > 0), "%hs: 잘못된 차감 요청. ItemId=%s, Quantity=%d", __FUNCTION__, *ItemId.ToString(), Quantity);
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

    AssignQuickSlot(0, TEXT("Potion.Health"));
    AssignQuickSlot(1, TEXT("Potion.Health.Large"));
}

FName UPriestInventorySubsystem::GetQuickSlotItemId(
    int32 SlotIndex
) const
{
    if (!QuickSlotItemIds.IsValidIndex(SlotIndex))
    {
        JError("%hs: 잘못된 퀵 슬롯 인덱스 %d. 슬롯 수=%d", __FUNCTION__, SlotIndex, QuickSlotItemIds.Num());
        return NAME_None;
    }

    return QuickSlotItemIds[SlotIndex];
}

bool UPriestInventorySubsystem::AssignQuickSlot(
    int32 SlotIndex,
    FName ItemId
)
{
    JASSERT_BOOL(
        (QuickSlotItemIds.IsValidIndex(SlotIndex)),
        "%hs: 잘못된 퀵 슬롯 인덱스 %d. 슬롯 수=%d",
        __FUNCTION__,
        SlotIndex,
        QuickSlotItemIds.Num()
    );

    if (!CanAssignQuickSlot(SlotIndex, ItemId))
    {
        return false;
    }

    if (QuickSlotItemIds[SlotIndex] == ItemId)
    {
        return true;
    }

    QuickSlotItemIds[SlotIndex] = ItemId;
    OnQuickSlotsChanged.Broadcast();

    return true;
}

bool UPriestInventorySubsystem::ClearQuickSlot(int32 SlotIndex)
{
    JASSERT_BOOL((QuickSlotItemIds.IsValidIndex(SlotIndex)), "%hs: 잘못된 퀵 슬롯 인덱스 %d. 슬롯 수=%d", __FUNCTION__, SlotIndex, QuickSlotItemIds.Num());

    if (QuickSlotItemIds[SlotIndex].IsNone())
    {
        return true;
    }

    QuickSlotItemIds[SlotIndex] = NAME_None;
    OnQuickSlotsChanged.Broadcast();

    return true;
}

void UPriestInventorySubsystem::Initialize(
    FSubsystemCollectionBase& Collection
)
{
    Super::Initialize(Collection);

    const UPriestInventorySettings* Settings =
        GetDefault<UPriestInventorySettings>();

    ItemDataTable = Settings->ItemDataTable.LoadSynchronous();

    JASSERT(
        IsValid(ItemDataTable),
        "%hs: ItemDataTable 로드 실패. 프로젝트 설정의 Inventory Settings를 확인하세요.",
        __FUNCTION__
    );

    if (
        ItemDataTable->GetRowStruct()
        != FPriestItemData::StaticStruct()
        )
    {
        JError(
            "%hs: DT_ItemData의 Row Structure가 "
            "PriestItemData가 아닙니다.",
            __FUNCTION__
        );

        ItemDataTable = nullptr;
        return;
    }

    PotionDataTable = Settings->PotionDataTable.LoadSynchronous();

    JASSERT(
        IsValid(PotionDataTable),
        "%hs: PotionDataTable 로드 실패. 프로젝트 설정의 Inventory Settings를 확인하세요.",
        __FUNCTION__
    );

    JASSERT(
        PotionDataTable->GetRowStruct()
        == FPotionData::StaticStruct(),
        "%hs: 포션 DataTable의 Row Structure가 "
        "PotionData가 아닙니다.",
        __FUNCTION__
    );
}

bool UPriestInventorySubsystem::TryGetPotionData(
    FName ItemId,
    FPotionData& OutData
) const
{
    OutData = FPotionData{};

    if (ItemId.IsNone())
    {
        return false;
    }

    JASSERT_BOOL(
        (IsValid(PotionDataTable)),
        "%hs: 포션 DataTable이 준비되지 않았습니다.",
        __FUNCTION__
    );

    const FPotionData* Data =
        PotionDataTable->FindRow<FPotionData>(
            ItemId,
            TEXT("TryGetPotionData"),
            false
        );

    if (!Data)
    {
        return false;
    }

    JASSERT_BOOL(
        (FMath::IsFinite(Data->HealAmount)
            && Data->HealAmount > 0.0f),
        "%hs: 포션 회복량이 잘못되었습니다. "
        "ItemId=%s, HealAmount=%f",
        __FUNCTION__,
        *ItemId.ToString(),
        Data->HealAmount
    );

    OutData = *Data;
    return true;
}

bool UPriestInventorySubsystem::CanAssignQuickSlot(
    int32 SlotIndex,
    FName ItemId
) const
{
    if (!QuickSlotItemIds.IsValidIndex(SlotIndex)
        || ItemId.IsNone()
        || GetQuantity(ItemId) <= 0)
    {
        return false;
    }

    FPotionData Data;
    return TryGetPotionData(ItemId, Data);
}

bool UPriestInventorySubsystem::TryGetItemData(
    FName ItemId,
    FPriestItemData& OutData
) const
{
    OutData = FPriestItemData{};

    if (ItemId.IsNone())
    {
        return false;
    }

    JASSERT_BOOL(
        (IsValid(ItemDataTable)),
        "%hs: 공통 아이템 DataTable이 준비되지 않았습니다.",
        __FUNCTION__
    );

    const FPriestItemData* Data =
        ItemDataTable->FindRow<FPriestItemData>(
            ItemId,
            TEXT("TryGetItemData"),
            false
        );

    if (!Data)
    {
        return false;
    }

    JASSERT_BOOL(
        (Data->Category != EItemCategory::None),
        "%hs: 아이템 분류가 미지정 상태입니다. ItemId=%s",
        __FUNCTION__,
        *ItemId.ToString()
    );

    OutData = *Data;
    return true;
}

TArray<FPriestOwnedItem>
UPriestInventorySubsystem::GetItemsByCategory(
    EItemCategory Category
) const
{
    TArray<FPriestOwnedItem> Result;

    if (Category == EItemCategory::None)
    {
        return Result;
    }

    JASSERT_RETURN(
        (IsValid(ItemDataTable)),
        Result,
        "%hs: 공통 아이템 DataTable이 준비되지 않았습니다.",
        __FUNCTION__
    );

    for (const FPriestOwnedItem& Item : OwnedItems)
    {
        FPriestItemData Data;

        if (!TryGetItemData(Item.ItemId, Data))
        {
            JError(
                "%hs: 보유 아이템의 공통 정의를 확인하세요. "
                "ItemId=%s",
                __FUNCTION__,
                *Item.ItemId.ToString()
            );

            continue;
        }

        if (Data.Category == Category)
        {
            Result.Add(Item);
        }
    }

    return Result;
}

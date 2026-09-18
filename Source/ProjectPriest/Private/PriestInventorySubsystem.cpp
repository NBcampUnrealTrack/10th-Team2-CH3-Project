#include "PriestInventorySubsystem.h"
#include "Engine/Engine.h"
#include "PriestInventorySettings.h"
#include "Engine/DataTable.h"
#include "UObject/UObjectGlobals.h"
#include "JUtility.h"

bool UPriestInventorySubsystem::AddItem(FName ItemId, int32 Quantity)
{
    FPriestItemData Data;

    JASSERT_BOOL((!ItemId.IsNone() && !Data.DisplayName.IsEmpty() && Quantity > 0), "%hs: 잘못된 추가 요청", __FUNCTION__)
    JASSERT_BOOL((TryGetItemData(ItemId, Data) && Data.MaxStack > 0), "%hs: 아이템 정의 또는 MaxStack을 확인하세요.", __FUNCTION__);

    for (FPriestOwnedItem& Item : OwnedItems)
    {
        if (Quantity == 0)
        {
            break;
        }

        if (Item.ItemId != ItemId || Item.Quantity >= Data.MaxStack)
        {
            continue;
        }

        const int32 Count = FMath::Min(Quantity, Data.MaxStack - Item.Quantity);

        Item.Quantity += Count;
        Quantity -= Count;
    }

    while (Quantity > 0)
    {
        FPriestOwnedItem Item;
        Item.ItemId = ItemId;
        Item.DisplayName = Data.DisplayName;
        Item.Quantity = FMath::Min(Quantity, Data.MaxStack);

        Quantity -= Item.Quantity;
        OwnedItems.Add(Item);
    }

    OnInventoryChanged.Broadcast();
    return true;
}

int32 UPriestInventorySubsystem::GetQuantity(FName ItemId) const
{
    int32 TotalQuantity = 0;

    for (const FPriestOwnedItem& Item : OwnedItems)
    {
        if (Item.ItemId == ItemId)
        {
            TotalQuantity += Item.Quantity;
        }
    }

    return TotalQuantity;
}

bool UPriestInventorySubsystem::RemoveItem(FName ItemId, int32 Quantity)
{
    JASSERT_BOOL((!ItemId.IsNone() && Quantity > 0), "%hs: 잘못된 차감 요청. ItemId=%s, Quantity=%d", __FUNCTION__, *ItemId.ToString(), Quantity);

    if (GetQuantity(ItemId) < Quantity)
    {
        return false;
    }

    int32 RemainingQuantity = Quantity;

    for (int32 Index = OwnedItems.Num() - 1; Index >= 0 && RemainingQuantity > 0; --Index)
    {
        FPriestOwnedItem& Item = OwnedItems[Index];

        if (Item.ItemId != ItemId)
        {
            continue;
        }

        const int32 RemovedQuantity = FMath::Min(Item.Quantity, RemainingQuantity);

        Item.Quantity -= RemovedQuantity;
        RemainingQuantity -= RemovedQuantity;

        if (Item.Quantity == 0)
        {
            OwnedItems.RemoveAt(Index);
        }
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
    AddItem(TEXT("HealthPotion"), 5);
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

    ItemDataTables.Reset();

    UDataTable* WeaponTable = Settings->WeaponDataTable.LoadSynchronous();

    JASSERT(
        IsValid(WeaponTable),
        "%hs: 무기 테이블 로드 실패. 프로젝트 설정을 확인하세요.",
        __FUNCTION__
    );

    JASSERT(
        WeaponTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "%hs: 무기 테이블의 Row Structure가 PriestItemData가 아닙니다.",
        __FUNCTION__
    );

    ItemDataTables.Add(EItemCategory::Weapon, WeaponTable);

    UDataTable* AmmoTable = Settings->AmmoDataTable.LoadSynchronous();

    JASSERT(
        IsValid(AmmoTable),
        "%hs: 탄약 테이블 로드 실패. 프로젝트 설정을 확인하세요.",
        __FUNCTION__
    );

    JASSERT(
        AmmoTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "%hs: 탄약 테이블의 Row Structure가 PriestItemData가 아닙니다.",
        __FUNCTION__
    );

    ItemDataTables.Add(EItemCategory::Ammo, AmmoTable);

    UDataTable* ConsumableTable = Settings->ConsumableDataTable.LoadSynchronous();

    JASSERT(
        IsValid(ConsumableTable),
        "%hs: 소모품 테이블 로드 실패. 프로젝트 설정을 확인하세요.",
        __FUNCTION__
    );

    JASSERT(
        ConsumableTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "%hs: 소모품 테이블의 Row Structure가 PriestItemData가 아닙니다.",
        __FUNCTION__
    );

    ItemDataTables.Add(EItemCategory::Consumable, ConsumableTable);

    UDataTable* MaterialTable = Settings->MaterialDataTable.LoadSynchronous();

    JASSERT(
        IsValid(MaterialTable),
        "%hs: 재료 테이블 로드 실패. 프로젝트 설정을 확인하세요.",
        __FUNCTION__
    );

    JASSERT(
        MaterialTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "%hs: 재료 테이블의 Row Structure가 PriestItemData가 아닙니다.",
        __FUNCTION__
    );

    ItemDataTables.Add(EItemCategory::Material, MaterialTable);
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

    bool bFound = false;
    FPriestItemData FoundData;

    for (const auto& Entry : ItemDataTables)
    {
        const UDataTable* Table = Entry.Value.Get();

        JASSERT_BOOL(
            (IsValid(Table)),
            "%hs: 유효하지 않은 아이템 테이블입니다. Category=%d",
            __FUNCTION__,
            static_cast<int32>(Entry.Key)
        );

        const FPriestItemData* Row =
            Table->FindRow<FPriestItemData>(
                ItemId,
                TEXT("TryGetItemData"),
                false
            );

        if (!Row)
        {
            continue;
        }

        JASSERT_BOOL(
            (!bFound),
            "%hs: 카테고리 테이블 간 ItemId가 중복됩니다. ItemId=%s",
            __FUNCTION__,
            *ItemId.ToString()
        );

        FoundData = *Row;
        FoundData.Category = Entry.Key;
        bFound = true;
    }

    if (!bFound)
    {
        return false;
    }

    OutData = FoundData;
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

    const TObjectPtr<UDataTable>* FoundTable = ItemDataTables.Find(Category);

    JASSERT_RETURN(
        (FoundTable != nullptr && IsValid(FoundTable->Get())),
        Result,
        "%hs: 카테고리의 데이터 테이블이 없습니다. Category=%d",
        __FUNCTION__,
        static_cast<int32>(Category)
    );

    const UDataTable* Table = FoundTable->Get();

    for (const FPriestOwnedItem& Item : OwnedItems)
    {
        const FPriestItemData* ItemData =
            Table->FindRow<FPriestItemData>(
                Item.ItemId,
                TEXT("GetItemsByCategory"),
                false
            );

        if (!ItemData)
        {
            continue;
        }

        Result.Add(Item);
    }

    return Result;
}

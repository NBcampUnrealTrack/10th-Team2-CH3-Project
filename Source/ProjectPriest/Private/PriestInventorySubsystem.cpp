#include "PriestInventorySubsystem.h"
#include "Engine/Engine.h"
#include "PriestInventorySettings.h"
#include "Engine/DataTable.h"
#include "UObject/UObjectGlobals.h"
#include "RecipeData.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "JUtility.h"

bool UPriestInventorySubsystem::AddItem(FName ItemId, int32 Quantity)
{
    FPriestItemData Data;

    JASSERT_BOOL((!ItemId.IsNone() && Quantity > 0), "잘못된 추가 요청")
    JASSERT_BOOL((TryGetItemData(ItemId, Data) && Data.MaxStack > 0), "아이템 정의 또는 MaxStack을 확인하세요.");

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

bool UPriestInventorySubsystem::CraftItem(FName RecipeID)
{
    JASSERT_BOOL(
        IsValid(RecipeDataTable),
        "RecipeDataTable이 준비되지 않았습니다."
    );

    const FRecipeData* Recipe = RecipeDataTable->FindRow<FRecipeData>(
        RecipeID,
        TEXT("UPriestInventorySubsystem::CraftItem"),
        false
    );

    JASSERT_BOOL(
        (Recipe != nullptr),
        "Recipe를 찾을 수 없습니다. RecipeID=%s",
        *RecipeID.ToString()
    );

    FPriestItemData ResultItemData;

    JASSERT_BOOL(
        TryGetItemData(Recipe->ResultItemID, ResultItemData),
        "레시피 데이터의 ResultItemID이 올바르지 않습니다."
    );

    for (const FRecipeIngredient& Ingredient : Recipe->Ingredients)
    {
        if (Ingredient.ItemID.IsNone()
            || Ingredient.Quantity <= 0
            || GetQuantity(Ingredient.ItemID) < Ingredient.Quantity)
        {
            return false;
        }
    }

    for (const FRecipeIngredient& Ingredient : Recipe->Ingredients)
    {
        if (!RemoveItem(Ingredient.ItemID, Ingredient.Quantity))
        {
            return false;
        }
    }

    return AddItem(Recipe->ResultItemID, Recipe->ResultQuantity);
}

bool UPriestInventorySubsystem::RemoveItem(FName ItemId, int32 Quantity)
{
    JASSERT_BOOL((!ItemId.IsNone() && Quantity > 0), "잘못된 차감 요청. ItemId=%s, Quantity=%d", *ItemId.ToString(), Quantity);

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

bool UPriestInventorySubsystem::EquipWeapon(FName ItemId)
{

    JASSERT_BOOL(!ItemId.IsNone(), "무기 아이템 Id가 없습니다");

    FPriestItemData ItemData;
    JASSERT_BOOL(TryGetItemData(ItemId, ItemData), "무기 선택 요청의 아이템 정의가 없습니다.");

    if (ItemData.Category != EItemCategory::Weapon || GetQuantity(ItemId) <= 0)
    {
        return false;
    }

    if (EquippedWeaponId == ItemId)
    {
        return true;
    }

    EquippedWeaponId = ItemId;
    OnEquippedWeaponChanged.Broadcast();

    return true;
}

FName UPriestInventorySubsystem::GetEquippedWeaponId() const
{
    return EquippedWeaponId;
}

void UPriestInventorySubsystem::GrantPreviewItemsOnce()
{
    if (bPreviewItemsGranted)
    {
        return;
    }
    bPreviewItemsGranted = true;
    AddItem(TEXT("HealthPotion"), 5);
    AddItem(TEXT("AttackSpeedUpPotion"), 2);
    AddItem(TEXT("WhisperDropItemA"), 3);
    AddItem(TEXT("WhisperDropItemB"), 2);
    AddItem(TEXT("Barrel"), 1);
    AddItem(TEXT("Magazine"), 1);
    AddItem(TEXT("Pistol"), 1);
    AddItem(TEXT("Crossbow"), 1);
}

FName UPriestInventorySubsystem::GetQuickSlotItemId(
    int32 SlotIndex
) const
{
    if (!QuickSlotItemIds.IsValidIndex(SlotIndex))
    {
        JError("잘못된 퀵 슬롯 인덱스 %d. 슬롯 수=%d", SlotIndex, QuickSlotItemIds.Num());
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
        "잘못된 퀵 슬롯 인덱스 %d. 슬롯 수=%d",
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
    JASSERT_BOOL((QuickSlotItemIds.IsValidIndex(SlotIndex)), "잘못된 퀵 슬롯 인덱스 %d. 슬롯 수=%d", SlotIndex, QuickSlotItemIds.Num());

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

    //회복 포션 데이터테이블
    HealthPotionDataTable = Settings->HealthPotionDataTable.LoadSynchronous();

    JASSERT(
        IsValid(HealthPotionDataTable),
        "PotionDataTable 로드 실패. 프로젝트 설정의 Inventory Settings를 확인하세요."
    );

    JASSERT(
        HealthPotionDataTable->GetRowStruct()
        == FHealthPotionData::StaticStruct(),
        "포션 DataTable의 Row Structure가 "
        "PotionData가 아닙니다."
    );

    //공격속도 증가 포션 데이터테이블
    AttackSpeedUpPotionDataTable = Settings->AttackSpeedUpPotionDataTable.LoadSynchronous();

    JASSERT(
        IsValid(AttackSpeedUpPotionDataTable),
        "PotionDataTable 로드 실패. 프로젝트 설정의 Inventory Settings를 확인하세요."
    );

    JASSERT(
        AttackSpeedUpPotionDataTable->GetRowStruct()
        == FAttackSpeedUpPotionData::StaticStruct(),
        "포션 DataTable의 Row Structure가 "
        "PotionData가 아닙니다."
    );

    ItemDataTables.Reset();

    //무기 데이터테이블
    UDataTable* WeaponTable = Settings->WeaponDataTable.LoadSynchronous();

    JASSERT(
        IsValid(WeaponTable),
        "무기 테이블 로드 실패. 프로젝트 설정을 확인하세요."
    );

    JASSERT(
        WeaponTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "무기 테이블의 Row Structure가 PriestItemData가 아닙니다."
    );

    ItemDataTables.Add(EItemCategory::Weapon, WeaponTable);

    //파츠 데이터테이블
    UDataTable* PartTable = Settings->PartDataTable.LoadSynchronous();

    JASSERT(
        IsValid(PartTable),
        "파츠 테이블 로드 실패. 프로젝트 설정을 확인하세요."
    );

    JASSERT(
        PartTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "파츠 테이블의 Row Structure가 PriestItemData가 아닙니다."
    );

    ItemDataTables.Add(EItemCategory::Part, PartTable);

    //소모품 데이터테이블
    UDataTable* ConsumableTable = Settings->ConsumableDataTable.LoadSynchronous();

    JASSERT(
        IsValid(ConsumableTable),
        "소모품 테이블 로드 실패. 프로젝트 설정을 확인하세요."
    );

    JASSERT(
        ConsumableTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "소모품 테이블의 Row Structure가 PriestItemData가 아닙니다."
    );

    ItemDataTables.Add(EItemCategory::Consumable, ConsumableTable);

    //재료 데이터테이블
    UDataTable* MaterialTable = Settings->MaterialDataTable.LoadSynchronous();

    JASSERT(
        IsValid(MaterialTable),
        "재료 테이블 로드 실패. 프로젝트 설정을 확인하세요."
    );

    JASSERT(
        MaterialTable->GetRowStruct() == FPriestItemData::StaticStruct(),
        "재료 테이블의 Row Structure가 PriestItemData가 아닙니다."
    );

    ItemDataTables.Add(EItemCategory::Material, MaterialTable);

    //레시피 데이터테이블
    RecipeDataTable = Settings->RecipeDataTable.LoadSynchronous();

    JASSERT(
        IsValid(RecipeDataTable),
        "RecipeDataTable 로드 실패. 프로젝트 설정을 확인하세요."
    );

    JASSERT(
        RecipeDataTable->GetRowStruct() == FRecipeData::StaticStruct(),
        "RecipeDataTable의 Row Structure가 RecipeData가 아닙니다."
    );
}

UPriestInventorySubsystem*
UPriestInventorySubsystem::Get(
    const UObject* WorldContextObject
)
{
    if (!IsValid(WorldContextObject))
    {
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!IsValid(World))
    {
        return nullptr;
    }

    UGameInstance* GameInstance = World->GetGameInstance();
    if (!IsValid(GameInstance))
    {
        return nullptr;
    }

    return GameInstance->GetSubsystem<UPriestInventorySubsystem>();
}

bool UPriestInventorySubsystem::TryGetHealthPotionData(
    FName ItemId,
    FHealthPotionData& OutData
) const
{
    OutData = FHealthPotionData{};

    if (ItemId.IsNone())
    {
        return false;
    }

    JASSERT_BOOL(
        (IsValid(HealthPotionDataTable)),
        "회복포션 DataTable이 준비되지 않았습니다."
    );

    const FHealthPotionData* Data =
        HealthPotionDataTable->FindRow<FHealthPotionData>(
            ItemId,
            TEXT("TryGetHealthPotionData"),
            false
        );

    if (!Data)
    {
        return false;
    }

    JASSERT_BOOL(
        (FMath::IsFinite(Data->HealAmount)
            && Data->HealAmount > 0.0f),
        "포션 회복량이 잘못되었습니다. "
        "ItemId=%s, HealAmount=%f",
        *ItemId.ToString(),
        Data->HealAmount
    );
    OutData = *Data;
    return true;
}

bool UPriestInventorySubsystem::TryGetAttackSpeedUpPotionData(
    FName ItemId,
    FAttackSpeedUpPotionData& OutData
) const
{
    OutData = FAttackSpeedUpPotionData{};

    if (ItemId.IsNone())
    {
        return false;
    }

    JASSERT_BOOL(
        (IsValid(AttackSpeedUpPotionDataTable)),
        "공격속도 증가포션 DataTable이 준비되지 않았습니다."
    );

    const FAttackSpeedUpPotionData* Data =
        AttackSpeedUpPotionDataTable
        ->FindRow<FAttackSpeedUpPotionData>(
            ItemId,
            TEXT("TryGetAttackSpeedUpPotionData"),
            false
        );

    if (!Data)
    {
        return false;
    }

    JASSERT_BOOL(
        (FMath::IsFinite(Data->AttackSpeedMultiplier)
            && Data->AttackSpeedMultiplier > 1.0f),
        "공격속도 배율이 잘못되었습니다. ItemId=%s",
        *ItemId.ToString()
    );

    JASSERT_BOOL(
        (FMath::IsFinite(Data->Duration)
            && Data->Duration > 0.0f),
        "지속시간이 잘못되었습니다. ItemId=%s",
        *ItemId.ToString()
    );

    OutData = *Data;

    return true;
}

bool UPriestInventorySubsystem::IsPotion(FName ItemId) const
{
    if (ItemId.IsNone())
    {
        return false;
    }

    JASSERT_BOOL(
        (IsValid(HealthPotionDataTable)),
        "회복포션 DataTable이 준비되지 않았습니다."
    );

    if (HealthPotionDataTable->FindRow<FHealthPotionData>(
        ItemId,
        TEXT("IsPotion"),
        false))
    {
        return true;
    }

    JASSERT_BOOL(
        (IsValid(AttackSpeedUpPotionDataTable)),
        "공격속도 증가포션 DataTable이 준비되지 않았습니다."
    );

    if (AttackSpeedUpPotionDataTable->FindRow<FAttackSpeedUpPotionData>(
        ItemId,
        TEXT("IsPotion"),
        false))
    {
        return true;
    }

    return false;
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

    return IsPotion(ItemId);
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
            "유효하지 않은 아이템 테이블입니다. Category=%d",
            StaticCast<int32>(Entry.Key)
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
            "카테고리 테이블 간 ItemId가 중복됩니다. ItemId=%s",
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
        "카테고리의 데이터 테이블이 없습니다. Category=%d",
        StaticCast<int32>(Category)
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

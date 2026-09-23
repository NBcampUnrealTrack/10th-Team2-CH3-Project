#include "PriestInventoryModel.h"
#include "PriestInventorySubsystem.h"
#include "MvcControl.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "Attachment/PartSubsystem.h"
#include "Engine/GameInstance.h"

void UPriestInventoryModel::Initialize(UPriestInventorySubsystem* InInventory)
{
    Disconnect();

    JASSERT(IsValid(InInventory), "%hs: InventorySubsystem이 유효하지 않습니다.", __FUNCTION__);

    UGameInstance* GameInstance = InInventory->GetGameInstance();
    JASSERT(IsValid(GameInstance), "GameInstance가 없습니다.");

    UPartSubsystem* PartSubsystem = GameInstance->GetSubsystem<UPartSubsystem>();
    JASSERT(IsValid(PartSubsystem), "PartSubsystem이 없습니다.");

    Inventory = InInventory;
    Parts = PartSubsystem;

    Inventory->OnInventoryChanged.AddUniqueDynamic(this, &UPriestInventoryModel::Refresh);
    Inventory->OnQuickSlotsChanged.AddUniqueDynamic(this, &UPriestInventoryModel::Refresh);
    Inventory->OnEquippedWeaponChanged.AddUniqueDynamic(this, &UPriestInventoryModel::Refresh);
    Parts->OnPartsChanged.AddUniqueDynamic(this, &UPriestInventoryModel::Refresh);

    Refresh();
}

void UPriestInventoryModel::Disconnect()
{
    if (IsValid(Inventory))
    {
        Inventory->OnInventoryChanged.RemoveDynamic(this, &UPriestInventoryModel::Refresh);
        Inventory->OnQuickSlotsChanged.RemoveDynamic(this, &UPriestInventoryModel::Refresh);
        Inventory->OnEquippedWeaponChanged.RemoveDynamic(this, &UPriestInventoryModel::Refresh);
    }

    if (IsValid(Parts))
    {
        Parts->OnPartsChanged.RemoveDynamic(this, &UPriestInventoryModel::Refresh);
    }

    Inventory = nullptr;
    Parts = nullptr;
}

void UPriestInventoryModel::BeginDestroy()
{
    Disconnect();
    Super::BeginDestroy();
}

bool UPriestInventoryModel::SetCategory(EItemCategory InCategory)
{
    if (!IsValid(Inventory))
    {
        return false;
    }

    switch (InCategory)
    {
        case EItemCategory::Weapon:
        case EItemCategory::Part:
        case EItemCategory::Consumable:
        case EItemCategory::Material:
            break;
        default:
            return false;
    }

    if (Data.SelectedCategory == InCategory)
    {
        return true;
    }

    Data.SelectedCategory = InCategory;

    Refresh();
    return true;
}

const FPriestInventoryViewData& UPriestInventoryModel::GetData() const
{
    return Data;
}

bool UPriestInventoryModel::EquipWeapon(FName ItemId)
{
    JASSERT_BOOL(IsValid(Inventory), "InventorySubsystem이 없습니다.");

    return Inventory->EquipWeapon(ItemId);
}

void UPriestInventoryModel::Refresh()
{
    JASSERT(IsValid(Inventory), "InventorySubsystem이 유효하지 않습니다.");
    JASSERT(IsValid(Parts), "PartSubSystem이 유효하지 않습니다.");

    Data.Items.Reset();

    const FName QuickSlot0 = Inventory->GetQuickSlotItemId(0);
    const FName QuickSlot1 = Inventory->GetQuickSlotItemId(1);

    const FName EquippedWeaponId = Inventory->GetEquippedWeaponId();

    FName BarrelItemId = NAME_None;
    FName MagazineItemId = NAME_None;

    if (Data.SelectedCategory == EItemCategory::Part && !EquippedWeaponId.IsNone())
    {
        BarrelItemId = Parts->GetEquippedPart(EquippedWeaponId, EPartSlot::Barrel).Name;
        MagazineItemId = Parts->GetEquippedPart(EquippedWeaponId, EPartSlot::Magazine).Name;
    }

    const TArray<FPriestOwnedItem> Items =
        Inventory->GetItemsByCategory(Data.SelectedCategory);

    for (const FPriestOwnedItem& Item : Items)
    {
        FPriestInventorySlotData SlotData;

        SlotData.ItemId = Item.ItemId;
        SlotData.DisplayName = Item.DisplayName;
        SlotData.Quantity = Item.Quantity;

        SlotData.Category = Data.SelectedCategory;

        SlotData.bQuickSlotCompatible =
            Inventory->IsPotion(Item.ItemId);

        JASSERT(!Item.ItemId.IsNone(), "아이템 Id가 잘못되었습니다.");

        if (SlotData.Category == EItemCategory::Weapon)
        {
            SlotData.bRegistered = Item.ItemId == EquippedWeaponId;
        }
        else if (SlotData.bQuickSlotCompatible)
        {
            SlotData.bRegistered = Item.ItemId == QuickSlot0 || Item.ItemId == QuickSlot1;
        }
        else if (SlotData.Category == EItemCategory::Part)
        {
            SlotData.bRegistered = Item.ItemId == BarrelItemId || Item.ItemId == MagazineItemId;
        }

        Data.Items.Add(MoveTemp(SlotData));
    }

    JLog("Refresh Called");
    InvokePropertyChanged(0);
}

FDelegateHandle UPriestInventoryModel::AddListener(UMvcControl* Control)
{
    return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}

void UPriestInventoryModel::RemoveListener(FDelegateHandle Handle)
{
    Changed.Remove(Handle);
}

void UPriestInventoryModel::InvokePropertyChanged(uint8 PropertyName)
{
    Changed.Broadcast(this, PropertyName);
}

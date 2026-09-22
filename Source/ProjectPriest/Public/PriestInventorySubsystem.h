#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PotionTypes.h"
#include "PriestItemTypes.h"
#include "PriestInventorySubsystem.generated.h"

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FPriestOwnedItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1")) int32 Quantity = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPriestInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPriestQuickSlotsChanged);

// Shared by the menu and gameplay worlds. Disk persistence is a separate concern.
UCLASS()
class PROJECTPRIEST_API UPriestInventorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(
        FSubsystemCollectionBase& Collection
    ) override;

    UFUNCTION(
        BlueprintPure,
        Category = "Priest|Inventory",
        meta = (WorldContext = "WorldContextObject")
    )
    static UPriestInventorySubsystem* Get(
        const UObject* WorldContextObject
    );

    UFUNCTION(BlueprintPure, Category="Priest|Inventory")
    TArray<FPriestOwnedItem> GetOwnedItems() const { return OwnedItems; }

    // ItemId is the stacking key; names are presentation only.
    UFUNCTION(BlueprintCallable, Category="Priest|Inventory")
    bool AddItem(FName ItemId, int32 Quantity);

    UFUNCTION(BlueprintPure, Category="Priest|Inventory")
    int32 GetQuantity(FName ItemId) const;

    UFUNCTION(BlueprintCallable, Category = "Priest|Inventory|Crafting")
    bool CraftItem(FName RecipeID);

    UFUNCTION(BlueprintCallable, Category="Priest|Inventory")
    bool RemoveItem(FName ItemId, int32 Quantity);

    // Call only from the temporary menu setup; never refill on menu re-entry.
    void GrantPreviewItemsOnce();

    UFUNCTION(BlueprintPure, Category = "Priest|Inventory|QuickSlot")
    FName GetQuickSlotItemId(int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Priest|Inventory|QuickSlot")
    bool AssignQuickSlot(int32 SlotIndex, FName ItemId);

    UFUNCTION(BlueprintCallable, Category = "Priest|Inventory|QuickSlot")
    bool ClearQuickSlot(int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Priest|Inventory|Potion")
    bool TryGetHealthPotionData(
        FName ItemId,
        FHealthPotionData& OutData
    ) const;

    bool TryGetAttackSpeedUpPotionData(
        FName ItemId,
        FAttackSpeedUpPotionData& OutData
    ) const;

    bool IsPotion(FName ItemId) const;

    UFUNCTION(BlueprintPure, Category = "Priest|Inventory|QuickSlot")
    bool CanAssignQuickSlot(
        int32 SlotIndex,
        FName ItemId
    ) const;

    UFUNCTION(BlueprintPure, Category = "Priest|Inventory|Data")
    bool TryGetItemData(
        FName ItemId,
        FPriestItemData& OutData
    ) const;

    UFUNCTION(BlueprintPure, Category = "Priest|Inventory")
    TArray<FPriestOwnedItem> GetItemsByCategory(
        EItemCategory Category
    ) const;

public:
    UPROPERTY(BlueprintAssignable, Category="Priest|Inventory")
    FPriestInventoryChanged OnInventoryChanged;

    UPROPERTY(BlueprintAssignable, Category = "Priest|Inventory|QuickSlot")
    FPriestQuickSlotsChanged OnQuickSlotsChanged;

private:
    UPROPERTY(Transient) TArray<FPriestOwnedItem> OwnedItems;
    bool bPreviewItemsGranted = false;

    UPROPERTY(Transient)
    TArray<FName> QuickSlotItemIds = { NAME_None, NAME_None };

    UPROPERTY(Transient)
    TObjectPtr<UDataTable> HealthPotionDataTable;

    UPROPERTY(Transient)
    TObjectPtr<UDataTable> AttackSpeedUpPotionDataTable;

    UPROPERTY(Transient)
    TObjectPtr<UDataTable> RecipeDataTable;

    UPROPERTY(Transient)
    TMap<EItemCategory, TObjectPtr<UDataTable>> ItemDataTables;
};

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
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

// Shared by the menu and gameplay worlds. Disk persistence is a separate concern.
UCLASS()
class PROJECTPRIEST_API UPriestInventorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="Priest|Inventory")
    TArray<FPriestOwnedItem> GetOwnedItems() const { return OwnedItems; }

    // ItemId is the stacking key; names are presentation only.
    UFUNCTION(BlueprintCallable, Category="Priest|Inventory")
    bool AddItem(FName ItemId, FText DisplayName, int32 Quantity);

    UFUNCTION(BlueprintPure, Category="Priest|Inventory")
    int32 GetQuantity(FName ItemId) const;

    UFUNCTION(BlueprintCallable, Category="Priest|Inventory")
    bool RemoveItem(FName ItemId, int32 Quantity);

    // Call only from the temporary menu setup; never refill on menu re-entry.
    void GrantPreviewItemsOnce();

    UPROPERTY(BlueprintAssignable, Category="Priest|Inventory")
    FPriestInventoryChanged OnInventoryChanged;

private:
    UPROPERTY(Transient) TArray<FPriestOwnedItem> OwnedItems;
    bool bPreviewItemsGranted = false;
};

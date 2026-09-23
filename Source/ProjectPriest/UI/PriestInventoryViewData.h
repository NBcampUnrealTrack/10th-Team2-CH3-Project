#pragma once

#include "CoreMinimal.h"
#include "PriestItemTypes.h"
#include "PriestInventoryViewData.generated.h"

USTRUCT(BlueprintType)
struct FPriestInventorySlotData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName ItemId = NAME_None;

    UPROPERTY(BlueprintReadOnly)
    FText DisplayName;

    UPROPERTY(BlueprintReadOnly)
    int32 Quantity = 0;

    UPROPERTY(BlueprintReadOnly)
    EItemCategory Category = EItemCategory::None;

    UPROPERTY(BlueprintReadOnly)
    bool bQuickSlotCompatible = false;

    UPROPERTY(BlueprintReadOnly)
    bool bRegistered = false;
};

USTRUCT(BlueprintType)
struct FPriestInventoryViewData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EItemCategory SelectedCategory = EItemCategory::Consumable;

    UPROPERTY(BlueprintReadOnly)
    TArray<FPriestInventorySlotData> Items;
};

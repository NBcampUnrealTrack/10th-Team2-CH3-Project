#pragma once
#include "CoreMinimal.h"
#include "PriestQuickSlotData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FPriestQuickSlotData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) int32 SlotIndex = 0;
    UPROPERTY(BlueprintReadOnly) FName ItemId = NAME_None;
    UPROPERTY(BlueprintReadOnly) FText DisplayName;
    UPROPERTY(BlueprintReadOnly) int32 Quantity = 0;
    UPROPERTY(BlueprintReadOnly) bool bAssigned = false;
};

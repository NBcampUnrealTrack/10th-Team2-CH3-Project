#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PriestItemTypes.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    None        UMETA(DisplayName = "미지정"),
    Weapon      UMETA(DisplayName = "무기"),
    Ammo        UMETA(DisplayName = "탄약"),
    Consumable  UMETA(DisplayName = "소모품"),
    Material    UMETA(DisplayName = "재료")
};

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FPriestItemData
    : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1", UIMin = "1"))
    int32 MaxStack = 99;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TObjectPtr<UTexture2D> Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemCategory Category = EItemCategory::None;
};

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PotionTypes.generated.h"

UENUM(BlueprintType)
enum class EPotionUseResult : uint8
{
    Success       UMETA(DisplayName = "사용 성공"),
    InvalidPotion UMETA(DisplayName = "잘못된 포션"),
    Unavailable   UMETA(DisplayName = "사용 불가"),
    Dead          UMETA(DisplayName = "사망 상태"),
    FullHealth    UMETA(DisplayName = "최대 체력"),
    NotOwned      UMETA(DisplayName = "수량 부족")
};

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FPotionDefinition : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Potion",
        meta = (ClampMin = "0.0")
    )
    float HealAmount = 30.0f;
};

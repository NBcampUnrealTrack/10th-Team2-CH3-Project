#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PotionTypes.generated.h"

UENUM(BlueprintType)
enum class EPotionEffectType : uint8
{
    Health      UMETA(DisplayName = "체력 회복"),
    AttackSpeed UMETA(DisplayName = "공격속도 증가")
};

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
struct PROJECTPRIEST_API FPotionData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
    EPotionEffectType EffectType = EPotionEffectType::Health;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Potion",
        meta = (
            ClampMin = "0.0",
            EditCondition = "EffectType == EPotionEffectType::Health",
            EditConditionHides
        )
    )
    float HealAmount = 30.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Potion",
        meta = (
            ClampMin = "1.0",
            EditCondition = "EffectType == EPotionEffectType::AttackSpeed",
            EditConditionHides
        )
    )
    float AttackSpeedMultiplier = 1.5f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadOnly,
        Category = "Potion",
        meta = (
            ClampMin = "0.0",
            EditCondition = "EffectType == EPotionEffectType::AttackSpeed",
            EditConditionHides
        )
    )
    float Duration = 10.0f;
};

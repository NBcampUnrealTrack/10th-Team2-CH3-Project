#pragma once

#include "CoreMinimal.h"
#include "MvcEvents.h"
#include "PriestItemTypes.h"
#include "PriestInventoryEventParameter.generated.h"

UENUM()
enum class EPriestInventoryAction : uint8
{
    SelectCategory,
    EquipWeapon
};

UCLASS()
class PROJECTPRIEST_API UPriestInventoryEventParameter
    : public UEventParameterBase
{
    GENERATED_BODY()

public:
    EPriestInventoryAction Action =
        EPriestInventoryAction::SelectCategory;

    EItemCategory Category =
        EItemCategory::Consumable;

    FName ItemId = NAME_None;

    bool bAccepted = false;
};

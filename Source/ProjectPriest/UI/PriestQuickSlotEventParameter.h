#pragma once

#include "CoreMinimal.h"
#include "MvcEvents.h"
#include "PriestQuickSlotEventParameter.generated.h"

UENUM()
enum class EPriestQuickSlotAction : uint8
{
    Assign,
    Clear
};

UCLASS()
class PROJECTPRIEST_API UPriestQuickSlotEventParameter
    : public UEventParameterBase
{
    GENERATED_BODY()

public:
    EPriestQuickSlotAction Action =
        EPriestQuickSlotAction::Assign;

    FName ItemId = NAME_None;

    bool bAccepted = false;
};

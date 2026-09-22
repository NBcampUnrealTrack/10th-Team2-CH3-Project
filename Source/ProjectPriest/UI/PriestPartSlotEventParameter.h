#pragma once

#include "CoreMinimal.h"
#include "MvcEvents.h"
#include "Attachment/PartData.h"
#include "PriestPartSlotEventParameter.generated.h"

UCLASS()
class PROJECTPRIEST_API UPriestPartSlotEventParameter
    : public UEventParameterBase
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FName ItemId = NAME_None;

    UPROPERTY()
    EPartSlot SlotType = EPartSlot::None;
};
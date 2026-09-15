#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "PriestInventoryDragDropOperation.generated.h"

UCLASS()
class PROJECTPRIEST_API UPriestInventoryDragDropOperation
    : public UDragDropOperation
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Priest|Inventory")
    FName ItemId = NAME_None;
};
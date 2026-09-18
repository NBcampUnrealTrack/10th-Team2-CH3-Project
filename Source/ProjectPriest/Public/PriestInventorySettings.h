#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PriestInventorySettings.generated.h"

class UDataTable;

UCLASS(
    Config = Game,
    DefaultConfig,
    meta = (DisplayName = "Inventory Settings")
)
class PROJECTPRIEST_API UPriestInventorySettings
    : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(
        Config,
        EditAnywhere,
        Category = "Data Tables"
    )
    TSoftObjectPtr<UDataTable> ItemDataTable;

    UPROPERTY(
        Config,
        EditAnywhere,
        Category = "Data Tables"
    )
    TSoftObjectPtr<UDataTable> PotionDataTable;
};

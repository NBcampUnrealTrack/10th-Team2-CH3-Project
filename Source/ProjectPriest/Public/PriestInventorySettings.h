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
    UPROPERTY(Config, EditAnywhere, Category = "Item Tables")
    TSoftObjectPtr<UDataTable> WeaponDataTable;

    UPROPERTY(Config, EditAnywhere, Category = "Item Tables")
    TSoftObjectPtr<UDataTable> AmmoDataTable;

    UPROPERTY(Config, EditAnywhere, Category = "Item Tables")
    TSoftObjectPtr<UDataTable> ConsumableDataTable;

    UPROPERTY(Config, EditAnywhere, Category = "Item Tables")
    TSoftObjectPtr<UDataTable> MaterialDataTable;

    UPROPERTY(Config, EditAnywhere, Category = "Effect Tables")
    TSoftObjectPtr<UDataTable> HealthPotionDataTable;

    UPROPERTY(Config, EditAnywhere, Category = "Effect Tables")
    TSoftObjectPtr<UDataTable> AttackSpeedUpPotionDataTable;
};

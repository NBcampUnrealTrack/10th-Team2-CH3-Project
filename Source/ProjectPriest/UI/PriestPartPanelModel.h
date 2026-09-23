#pragma once

#include "CoreMinimal.h"
#include "MvcModel.h"
#include "Attachment/PartData.h"
#include "PriestPartPanelModel.generated.h"

class UDataTable;
class UPartSubsystem;
class UPriestInventorySubsystem;

UCLASS()
class PROJECTPRIEST_API UPriestPartPanelModel: public UObject, public IMvcModel
{
    GENERATED_BODY()

public:
    void Initialize(
        UPartSubsystem* InParts,
        UPriestInventorySubsystem* InInventory,
        UDataTable* InPartTable
    );

    void Disconnect();

    bool EquipPart(FName ItemId, EPartSlot SlotType);

    FPartData GetPart(EPartSlot SlotType) const;

    virtual void BeginDestroy() override;

    virtual FDelegateHandle AddListener(UMvcControl* Control) override;

    virtual void RemoveListener(FDelegateHandle Handle) override;

    virtual void InvokePropertyChanged(uint8 PropertyName) override;

private:
    UFUNCTION()
    void HandleEquippedWeaponChanged();

private:
    UPROPERTY(Transient)
    TObjectPtr<UPartSubsystem> Parts;

    UPROPERTY(Transient)
    TObjectPtr<UPriestInventorySubsystem> Inventory;

    UPROPERTY(Transient)
    TObjectPtr<UDataTable> PartTable;

    FName WeaponId = NAME_None;

    FModelChangedDelegate Changed;
};
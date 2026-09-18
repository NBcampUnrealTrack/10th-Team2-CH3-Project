#pragma once
#include "CoreMinimal.h"
#include "MvcModel.h"
#include "PriestQuickSlotData.h"
#include "PriestQuickSlotModel.generated.h"

class UPriestInventorySubsystem;

UCLASS()
class PROJECTPRIEST_API UPriestQuickSlotModel : public UObject, public IMvcModel
{
    GENERATED_BODY()
public:
    void Initialize(UPriestInventorySubsystem* InInventory, int32 InSlotIndex);
    void Disconnect();
    bool AssignItem(FName ItemID);
    bool ClearItem();
    virtual void BeginDestroy() override;
    const FPriestQuickSlotData& GetData() const;
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokePropertyChanged(uint8 PropertyName) override;
private:
    UFUNCTION() void Refresh();

    UPROPERTY(Transient) TObjectPtr<UPriestInventorySubsystem> Inventory;
    UPROPERTY(Transient) FPriestQuickSlotData Data;
    FModelChangedDelegate Changed;
};

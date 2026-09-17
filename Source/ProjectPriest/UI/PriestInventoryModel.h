#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MvcModel.h"
#include "PriestInventoryViewData.h"
#include "PriestInventoryModel.generated.h"

class UPriestInventorySubsystem;
class UMvcControl;

UCLASS()
class PROJECTPRIEST_API UPriestInventoryModel: public UObject , public IMvcModel
{
    GENERATED_BODY()

public:
    void Initialize(UPriestInventorySubsystem* InInventory);

    void Disconnect();

    bool SetCategory(EItemCategory InCategory);

    const FPriestInventoryViewData& GetData() const
    {
        return Data;
    }

    virtual FDelegateHandle AddListener(UMvcControl* Control) override;

    virtual void RemoveListener(FDelegateHandle Handle) override;

    virtual void InvokePropertyChanged(uint8 PropertyName) override;

protected:
    virtual void BeginDestroy() override;

private:
    UFUNCTION()
    void Refresh();

private:
    UPROPERTY(Transient)
    TObjectPtr<UPriestInventorySubsystem> Inventory;

    UPROPERTY(Transient)
    FPriestInventoryViewData Data;

    FModelChangedDelegate Changed;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PriestQuickSlotWidget.generated.h"

class UPriestInventorySubsystem;
class UTextBlock;

// WBP owns layout and styling; this class owns inventory observation and text updates.
UCLASS(Abstract, Blueprintable)
class PROJECTPRIEST_API UPriestQuickSlotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Priest|QuickSlot")
    void SetSlotIndex(int32 InSlotIndex);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // Configure each instance in the HUD Designer: 0 = key 1, 1 = key 2.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|QuickSlot", meta = (ClampMin = "0", ClampMax = "1", ExposeOnSpawn = "true"))
    int32 SlotIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|QuickSlot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EmptyOpacity = 0.4f;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> KeyText;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ItemNameText;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> QuantityText;

private:
    UFUNCTION() void RefreshQuickSlot();
    void DisconnectInventory();

    UPROPERTY(Transient) TObjectPtr<UPriestInventorySubsystem> Inventory;
    FName LastItemId;
    FText LastItemName;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PriestInventorySubsystem.h"
#include "PriestInventorySlotWidget.generated.h"

class UTextBlock;
class UDragDropOperation;

// Create WBP_InventorySlot from this parent. All appearance belongs to its Designer.
UCLASS(Abstract, Blueprintable)
class PROJECTPRIEST_API UPriestInventorySlotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Priest|Inventory")
    void SetItem(const FPriestOwnedItem& InItem);

protected:
    virtual void NativePreConstruct() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Priest|Inventory")
    void OnItemChanged(const FPriestOwnedItem& InItem);

    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent
    ) override;

    virtual void NativeOnDragDetected(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation
    ) override;

private:
    void RefreshItem();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|Inventory")
    FPriestOwnedItem Item;

    // Required names in the slot WBP; font, color and alignment are designer-owned.
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ItemNameText;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> QuantityText;
};

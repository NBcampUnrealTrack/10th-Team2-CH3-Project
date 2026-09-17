#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "PriestQuickSlotData.h"
#include "PriestQuickSlotWidget.generated.h"

class UTextBlock;
class UDragDropOperation;
class UPriestUIManager;

// WBP owns layout and styling; this class owns inventory observation and text updates.
UCLASS(Abstract, Blueprintable)
class PROJECTPRIEST_API UPriestQuickSlotWidget : public UUserWidget, public IMvcView
{
    GENERATED_BODY()

public:
    void SetQuickSlotData(const FPriestQuickSlotData& InData);
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter) override;
    UFUNCTION(BlueprintCallable, Category = "Priest|QuickSlot")
    void SetSlotIndex(int32 InSlotIndex);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    virtual bool NativeOnDrop(
        const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation
    ) override;

private:
    void RefreshDisplay();

    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent
    ) override;

protected:

    // Configure each instance in the HUD Designer: 0 = key 1, 1 = key 2.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|QuickSlot", meta = (ClampMin = "0", ClampMax = "1", ExposeOnSpawn = "true"))
    int32 SlotIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|QuickSlot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EmptyOpacity = 0.4f;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> KeyText;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ItemNameText;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> QuantityText;

private:
    UPROPERTY(Transient) FPriestQuickSlotData ViewData;
    TWeakObjectPtr<UPriestUIManager> UIManager;
    FViewEventRaisedDelegate Listener;

};

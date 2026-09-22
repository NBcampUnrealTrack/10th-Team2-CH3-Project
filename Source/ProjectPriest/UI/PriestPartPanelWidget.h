#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "Attachment/PartData.h"
#include "PriestPartPanelWidget.generated.h"
#include "MvcUtility.h"

IMPLEMENT_VIEW_DEFAULT_ADDLISTENER(UPriestPartPanelWidget)
IMPLEMENT_VIEW_DEFAULT_REMOVELISTENER(UPriestPartPanelWidget)
IMPLEMENT_VIEW_DEFAULT_INVOKE_VIEW_EVENT(UPriestPartPanelWidget)

class UDataTable;
class UPriestPartPanelModel;
class UPriestPartPanelController;

UCLASS(Abstract, Blueprintable)
class PROJECTPRIEST_API UPriestPartPanelWidget: public UUserWidget, public IMvcView
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Priest|Part")
    void SetWeaponId(FName InWeaponId);

    UFUNCTION(BlueprintCallable, Category = "Priest|Part")
    void RequestEquipPart(FName ItemId, EPartSlot SlotType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Priest|Part")
    void UpdatePartSlots(const FPartData& BarrelPart, const FPartData& MagazinePart);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|Part")
    FName WeaponId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Priest|Part")
    TObjectPtr<UDataTable> PartDefinitionTable;

private:
    UPROPERTY(Transient)
    TObjectPtr<UPriestPartPanelModel> Model;

    UPROPERTY(Transient)
    TObjectPtr<UPriestPartPanelController> Controller;

    FViewEventRaisedDelegate ViewEventListeners;
};

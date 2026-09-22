#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "Attachment/PartData.h"
#include "MvcUtility.h"
#include "PriestPartPanelWidget.generated.h"

class UDataTable;
class UPriestPartPanelModel;

UCLASS(Abstract, Blueprintable)
class PROJECTPRIEST_API UPriestPartPanelWidget: public UUserWidget, public IMvcView
{
    GENERATED_BODY()

    DECLARE_VIEW_DEFAULT_INTERFACES()

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
};

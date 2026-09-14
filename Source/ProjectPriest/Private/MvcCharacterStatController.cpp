#include "MvcCharacterStatController.h"
#include "PriestCombatModel.h"
#include "../UI/PriestHUDWidget.h"
UMvcCharacterStatController::UMvcCharacterStatController() {}
void UMvcCharacterStatController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) {}
void UMvcCharacterStatController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestCombatModel* Combat = GetModel<UPriestCombatModel>();
    UPriestHUDWidget* Widget = GetView<UPriestHUDWidget>();
    if (!Combat || !Widget || InModel != Combat) return;
    const FPriestHUDData Data = Combat->GetCombatData();
    Widget->SetCombatData(Data.Health, Data.MaxHealth, Data.WeaponName, Data.MagazineAmmo, Data.ReserveAmmo);
}

#include "MvcCharacterStatController.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "PriestCombatModel.h"
#include "../UI/PriestHUDWidget.h"
UMvcCharacterStatController::UMvcCharacterStatController()
{
}
void UMvcCharacterStatController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
}
void UMvcCharacterStatController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    UPriestCombatModel* Combat = GetModel<UPriestCombatModel>();
    UPriestHUDWidget* Widget = GetView<UPriestHUDWidget>();
    JASSERT((IsValid(Combat)), "%hs [%s]: Combat model is invalid. Connect PriestCombatModel before updating HUD.", __FUNCTION__, *GetNameSafe(this));
    JASSERT((IsValid(Widget)), "%hs [%s]: Combat HUD view is invalid. Connect PriestHUDWidget before updating HUD.", __FUNCTION__, *GetNameSafe(this));
    if (InModel != Combat)
    {
        return;
    }
    const FPriestHUDData Data = Combat->GetCombatData();
    Widget->SetCombatData(Data.Health, Data.MaxHealth, Data.WeaponName, Data.MagazineAmmo, Data.ReserveAmmo);
}

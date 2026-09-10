#include "MvcCharacterStatController.h"
#include "PlayerCharacter.h"
#include "JUtility.h"
#include "../UI/PriestHUDWidget.h"

UMvcCharacterStatController::UMvcCharacterStatController()
{
}

void UMvcCharacterStatController::HandleViewEvent(IMvcView* View, EViewEventType EventType, UEventParameterBase* Parameter)
{
}

void UMvcCharacterStatController::HandleModelChanged(IMvcModel* Model, uint8 PropertyName)
{
    JASSERT(nullptr != Model, "Model is invalid");
    JASSERT(nullptr != View, "View is invalid");

    APlayerCharacter* PlayerCharacter
        = Cast<APlayerCharacter>(Model);

    JASSERT(IsValid(PlayerCharacter), "Model is not APlayerCharacter");

    UPriestHUDWidget* Widget
        = Cast<UPriestHUDWidget>(View);

    JASSERT(IsValid(PlayerCharacter), "View is not UPriestHUDWidget");

    PlayerCharacter->hela
}

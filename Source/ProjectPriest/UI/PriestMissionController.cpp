#include "PriestMissionController.h"
#include "IngameGameState.h"
#include "PriestHUDWidget.h"

FText UPriestMissionController::GetMissionText() const
{
    const AIngameGameState* State = Cast<AIngameGameState>(ModelObject.Get());
    if (!State) return NSLOCTEXT("PriestMission", "Waiting", "Loading mission...");
    const FText Count = FText::Format(NSLOCTEXT("PriestMission", "Remaining", "Enemies remaining: {0}"), FText::AsNumber(State->GetMonsterCount()));
    if (State->GetMonsterCount() == 0 && State->IsExitAvailable())
    {
        return FText::Format(NSLOCTEXT("PriestMission", "ExitReady", "{0}\nInteract at the objective area."), Count);
    }
    return Count;
}

void UPriestMissionController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
    if (InModel != GetModel<AIngameGameState>()) return;
    if (UPriestHUDWidget* View = GetView<UPriestHUDWidget>()) View->SetMissionObjective(GetMissionText());
}

#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "IngameGameState.h"
#include "../../UI/PriestMissionController.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPriestMissionTest, "Priest.UI.MVC.MissionProgress",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPriestMissionTest::RunTest(const FString& Parameters)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    if (!TestNotNull(TEXT("Test world"), World)) return false;
    AIngameGameState* State = World->SpawnActor<AIngameGameState>();
    AIngameGameState* OtherState = World->SpawnActor<AIngameGameState>();
    if (!TestNotNull(TEXT("Mission state"), State) || !TestNotNull(TEXT("Replacement state"), OtherState))
    {
        World->DestroyWorld(false);
        return false;
    }
    TStrongObjectPtr<UPriestMissionController> Controller(NewObject<UPriestMissionController>());
    const FString Waiting = Controller->GetMissionText().ToString();
    Controller->SetModel(State);
    State->SetMonsterCount(2);
    TestTrue(TEXT("Shows current enemy count on initial connection"), Controller->GetMissionText().ToString().Contains(TEXT("2")));
    State->IncreaseMosnterCount();
    TestEqual(TEXT("Spawn adds one enemy"), State->GetMonsterCount(), 3);
    State->DecreaseMosnterCount();
    State->DecreaseMosnterCount();
    State->DecreaseMosnterCount();
    const FString BeforeUnlock = Controller->GetMissionText().ToString();
    State->SetDoorVisibility(true);
    TestNotEqual(TEXT("Unlock adds interaction guidance without auto-clearing"), Controller->GetMissionText().ToString(), BeforeUnlock);
    State->DecreaseMosnterCount();
    TestEqual(TEXT("Repeated removal never displays a negative count"), State->GetMonsterCount(), 0);
    OtherState->SetMonsterCount(7);
    Controller->SetModel(OtherState);
    State->SetMonsterCount(99);
    TestTrue(TEXT("Replacing state ignores previous world's count"), Controller->GetMissionText().ToString().Contains(TEXT("7")));
    Controller->SetModel(nullptr);
    TestEqual(TEXT("Missing state returns waiting instead of zero enemies"), Controller->GetMissionText().ToString(), Waiting);
    Controller->Disconnect();
    World->DestroyWorld(false);
    return true;
}
#endif

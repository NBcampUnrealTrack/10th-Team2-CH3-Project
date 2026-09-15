#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "IngameGameState.h"
#include "IngamePlayerController.h"
#include "../../UI/PriestStageClearController.h"
#include "../../UI/PriestStageClearWidget.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPriestStageClearTest, "Priest.UI.MVC.StageClear",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPriestStageClearTest::RunTest(const FString& Parameters)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    if (!TestNotNull(TEXT("World"), World))
    {
        return false;
    }
    AIngameGameState* State = World->SpawnActor<AIngameGameState>();
    AIngameGameState* Other = World->SpawnActor<AIngameGameState>();
    AIngamePlayerController* Owner = World->SpawnActor<AIngamePlayerController>();
    if (!TestNotNull(TEXT("State"), State) || !TestNotNull(TEXT("Other"), Other) || !TestNotNull(TEXT("Owner"), Owner))
    {
        World->DestroyWorld(false);
        return false;
    }
    TStrongObjectPtr<UPriestStageClearController> Controller(NewObject<UPriestStageClearController>());
    Controller->Initialize(State, nullptr, Owner);
    TestTrue(TEXT("Travel failure listener connected"), Owner->OnResultTravelFailed.IsBoundToObject(Controller.Get()));
    TestFalse(TEXT("Initial zero enemies cannot clear locked objective"), State->TryCompleteStage(12.0f));
    State->SetMonsterCount(1);
    State->SetDoorVisibility(true);
    TestFalse(TEXT("Enemies prevent clear even if objective is enabled"), State->TryCompleteStage(12.0f));
    State->SetMonsterCount(0);
    TestFalse(TEXT("Last enemy does not auto-clear"), State->HasStageCleared());
    TestFalse(TEXT("Negative time rejected"), State->TryCompleteStage(-1.0f));
    TestTrue(TEXT("Valid interaction records clear"), State->TryCompleteStage(207.9f));
    TestTrue(TEXT("MVC observes clear"), Controller->IsStageClearActive());
    TestFalse(TEXT("Objective is disabled after clear"), State->IsExitAvailable());
    TestFalse(TEXT("Repeated interaction rejected"), State->TryCompleteStage(999.0f));
    State->SetElapsedTime(999.0f);
    State->SetMonsterCount(5);
    State->SetDoorVisibility(true);
    TestEqual(TEXT("Clear time remains frozen"), State->GetElapsedTime(), 207.9f);
    TestEqual(TEXT("Terminal count remains frozen"), State->GetMonsterCount(), 0);
    TestFalse(TEXT("Terminal objective remains disabled"), State->IsExitAvailable());
    TestEqual(TEXT("Whole-second time formatting"), UPriestStageClearWidget::FormatClearTime(207.9f).ToString(), FString(TEXT("Clear time: 00:03:27")));
    TestEqual(TEXT("Hours do not wrap at 60 minutes"), UPriestStageClearWidget::FormatClearTime(3661.0f).ToString(), FString(TEXT("Clear time: 01:01:01")));
    Controller->HandleModelChanged(Other, 0);
    TestTrue(TEXT("Foreign model notification ignored"), Controller->IsStageClearActive());
    Controller->Initialize(State, nullptr, Owner);
    TestTrue(TEXT("Already-cleared model synchronizes on connection"), Controller->IsStageClearActive());
    UMvcControl* Base = Controller.Get();
    Base->Disconnect();
    TestFalse(TEXT("Base disconnect clears derived state"), Controller->IsStageClearActive());
    TestFalse(TEXT("Travel failure listener removed"), Owner->OnResultTravelFailed.IsBoundToObject(Controller.Get()));
    State->InvokePropertyChanged(0);
    TestFalse(TEXT("Disconnected controller ignores old model"), Controller->IsStageClearActive());
    World->DestroyWorld(false);
    return true;
}
#endif

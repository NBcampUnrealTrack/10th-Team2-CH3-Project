#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "MvcCharacterStatController.h"
#include "PriestCombatModel.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/GarbageCollection.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPriestMvcLifetimeTest, "Priest.UI.MVC.ConnectionLifetime",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPriestMvcLifetimeTest::RunTest(const FString& Parameters)
{
    TStrongObjectPtr<UMvcCharacterStatController> Controller(NewObject<UMvcCharacterStatController>());
    Controller->Disconnect();
    Controller->Disconnect();
    TestNull(TEXT("Unconnected controller accepts repeated disconnect"), Controller->GetModel<UPriestCombatModel>());

    TStrongObjectPtr<UPriestCombatModel> First(NewObject<UPriestCombatModel>());
    TStrongObjectPtr<UPriestCombatModel> Second(NewObject<UPriestCombatModel>());
    Controller->SetModel(First.Get());
    Controller->SetModel(First.Get());
    TestTrue(TEXT("Repeated connection keeps the current model"), Controller->GetModel<UPriestCombatModel>() == First.Get());
    Controller->SetModel(Second.Get());
    TestTrue(TEXT("Replacement selects the new model"), Controller->GetModel<UPriestCombatModel>() == Second.Get());
    First->SetPawn(nullptr);
    Second->SetPawn(nullptr); // No view is a supported initialization state.
    TestEqual(TEXT("Missing pawn displays zero health"), Second->GetCombatData().Health, 0.0f);
    TestEqual(TEXT("Missing weapon displays zero ammo"), Second->GetCombatData().MagazineAmmo, 0);

    TWeakObjectPtr<UPriestCombatModel> Expiring = Second.Get();
    Second.Reset();
    CollectGarbage(RF_NoFlags);
    TestFalse(TEXT("Controller does not keep an externally owned model alive"), Expiring.IsValid());
    TestNull(TEXT("Collected model resolves to null"), Controller->GetModel<UPriestCombatModel>());
    Controller->Disconnect(); // Must not dereference the expired interface.
    Controller->SetModel(First.Get());
    TestTrue(TEXT("Controller reconnects after model destruction"), Controller->GetModel<UPriestCombatModel>() == First.Get());
    Controller->Disconnect();
    return true;
}
#endif

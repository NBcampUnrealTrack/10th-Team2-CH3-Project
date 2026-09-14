#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DamageEvents.h"
#include "PlayerCharacter.h"
#include "PriestCombatModel.h"
#include "../../UI/PriestDeathController.h"
#include "IngamePlayerController.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPriestDeathModelTest, "Priest.UI.MVC.DeathState",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPriestDeathModelTest::RunTest(const FString& Parameters)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    if (!TestNotNull(TEXT("Test world"), World)) return false;
    // APawn only accepts damage in a world with an authority GameMode.
    TStrongObjectPtr<UGameInstance> GameInstance(NewObject<UGameInstance>());
    World->SetGameInstance(GameInstance.Get());
    World->GetWorldSettings()->DefaultGameMode = AGameModeBase::StaticClass();
    if (!TestTrue(TEXT("Authority game mode"), World->SetGameMode(FURL())))
    {
        World->DestroyWorld(false);
        return false;
    }
    FActorSpawnParameters Spawn;
    Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    APlayerCharacter* First = World->SpawnActor<APlayerCharacter>(FVector::ZeroVector, FRotator::ZeroRotator, Spawn);
    APlayerCharacter* Second = World->SpawnActor<APlayerCharacter>(FVector(1000, 0, 0), FRotator::ZeroRotator, Spawn);
    if (!TestNotNull(TEXT("First player"), First) || !TestNotNull(TEXT("Second player"), Second))
    {
        World->DestroyWorld(false);
        return false;
    }
    TStrongObjectPtr<UPriestCombatModel> Model(NewObject<UPriestCombatModel>());
    TestFalse(TEXT("No pawn does not mean death"), Model->IsPlayerDead());
    Model->SetPawn(First);
    TStrongObjectPtr<UPriestDeathController> DeathController(NewObject<UPriestDeathController>());
    AIngamePlayerController* Owner = World->SpawnActor<AIngamePlayerController>();
    DeathController->Initialize(Model.Get(), nullptr, Owner);
    TestTrue(TEXT("Explicit executor receives failure subscription"), Owner->OnResultTravelFailed.IsBoundToObject(DeathController.Get()));
    DeathController->Disconnect();
    TestFalse(TEXT("Disconnect removes executor subscription"), Owner->OnResultTravelFailed.IsBoundToObject(DeathController.Get()));
    // State transitions work independently of a UIManager Outer and optional presentation.
    DeathController->Initialize(Model.Get(), nullptr, nullptr);
    TestFalse(TEXT("Controller initially observes alive state"), DeathController->IsDeathActive());
    TestFalse(TEXT("Healthy pawn is alive"), Model->IsPlayerDead());
    First->TakeDamage(25.0f, FDamageEvent(), nullptr, nullptr);
    TestFalse(TEXT("Nonlethal damage keeps player alive"), Model->IsPlayerDead());
    First->TakeDamage(1000.0f, FDamageEvent(), nullptr, nullptr);
    TestTrue(TEXT("Lethal damage is reflected by the model"), Model->IsPlayerDead());
    TestTrue(TEXT("MVC controller enters death on the model event"), DeathController->IsDeathActive());
    DeathController->Initialize(Model.Get(), nullptr, nullptr);
    TestTrue(TEXT("Reinitialization synchronizes an already dead model immediately"), DeathController->IsDeathActive());
    TStrongObjectPtr<UPriestCombatModel> ForeignModel(NewObject<UPriestCombatModel>());
    DeathController->HandleModelChanged(ForeignModel.Get(), 0);
    TestTrue(TEXT("An unrelated model cannot clear active death"), DeathController->IsDeathActive());
    First->TakeDamage(10.0f, FDamageEvent(), nullptr, nullptr);
    TestTrue(TEXT("Repeated damage remains dead"), Model->IsPlayerDead());
    Model->SetPawn(Second);
    TestFalse(TEXT("Possessing a healthy pawn clears death"), Model->IsPlayerDead());
    TestFalse(TEXT("MVC controller exits death on pawn replacement"), DeathController->IsDeathActive());
    First->OnCombatChanged.Broadcast();
    TestFalse(TEXT("Old pawn events cannot restore old death state"), Model->IsPlayerDead());
    Model->SetPawn(nullptr);
    TestFalse(TEXT("Unpossessing is not a death"), Model->IsPlayerDead());
    Model->SetPawn(First);
    TestTrue(TEXT("Rebinding a dead pawn enters death"), DeathController->IsDeathActive());
    UMvcControl* BaseController = DeathController.Get();
    BaseController->Disconnect();
    TestFalse(TEXT("Base disconnect clears derived death state"), DeathController->IsDeathActive());
    Model->InvokePropertyChanged(0);
    TestFalse(TEXT("Disconnected controller ignores later model notifications"), DeathController->IsDeathActive());
    DeathController->Disconnect();
    Model->Disconnect();
    World->DestroyWorld(false);
    return true;
}
#endif

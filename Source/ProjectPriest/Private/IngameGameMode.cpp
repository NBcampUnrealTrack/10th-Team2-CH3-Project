#include "IngameGameMode.h"
#include "IngameGameState.h"
#include "PlayerCharacter.h"
#include "BossRoomOpenInteractor.h"
#include "JUtility.h"

void AIngameGameMode::BeginPlay()
{
    Super::BeginPlay();

}

void AIngameGameMode::InitGameState()
{
    Super::InitGameState();

    JASSERT(IsValid(GameState), "Game mode is not valid");

    IngameState = Cast<AIngameGameState>(GameState);
    JASSERT(IsValid(IngameState), "Game stae is not AIngameGameState");

    IngameState->SetStartTime(GetWorld()->TimeSeconds);
    IngameState->SetMonsterCount(0);
}

void AIngameGameMode::OnPlayerDead()
{
    JError("플레이어 사망 화면을 구현하세요");
}

void AIngameGameMode::OnOpenBossRoomDoor(APlayerCharacter* Player)
{
    if (!IsValid(IngameState) || !IsValid(Player) || Player->GetWorld() != GetWorld()
        || !FMath::IsFinite(Player->GetCurrentHealth()) || Player->GetCurrentHealth() <= 0.0f
        || !IngameState->IsExitAvailable() || IngameState->GetMonsterCount() != 0) return;
    TArray<AActor*> Objectives;
    Player->GetOverlappingActors(Objectives, ABossRoomOpenInteractor::StaticClass());
    if (Objectives.IsEmpty()) return;
    IngameState->TryCompleteStage(GetWorld()->GetTimeSeconds() - IngameState->GetStartTime());
}

void AIngameGameMode::OnMonsterSpawned()
{
    if (!IsValid(IngameState)) return;
    IngameState->SetDoorVisibility(false);
    IngameState->IncreaseMosnterCount();
}

void AIngameGameMode::OnMonsterDead()
{
    if (!IsValid(IngameState)) return;
    IngameState->DecreaseMosnterCount();
    if (IngameState->GetMonsterCount() <= 0 )
    {
        IngameState->SetDoorVisibility(true);
    }
}

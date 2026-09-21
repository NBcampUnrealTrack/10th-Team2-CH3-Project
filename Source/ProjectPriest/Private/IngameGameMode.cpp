#include "IngameGameMode.h"
#include "IngameGameState.h"
#include "PlayerCharacter.h"
#include "BossRoomOpenInteractor.h"
#include "JUtility.h"
#include "Kismet/GameplayStatics.h"

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

    // 사망 상태 처리
    AIngameGameState* IngameGameState = GetWorld()->GetGameState<AIngameGameState>();

    JASSERT(IngameGameState, "IngameGameState가 없습니다.");

    IngameGameState->OnGameOver();
}

void AIngameGameMode::OnOpenBossRoomDoor(APlayerCharacter* Player)
{
    //TODO: 좀 더 보기 편한 구조로 바꾸자.
    if (!IsValid(IngameState) || !IsValid(Player) || Player->GetWorld() != GetWorld()
        || !FMath::IsFinite(Player->GetCurrentHealth()) || Player->GetCurrentHealth() <= 0.0f
        || !IngameState->IsExitAvailable() || IngameState->GetMonsterCount() != 0)
    {
        return;
    }
    TArray<AActor*> Objectives;
    Player->GetOverlappingActors(Objectives, ABossRoomOpenInteractor::StaticClass());
    if (Objectives.IsEmpty())
    {
        return;
    }
    
    JLog("Try to load boss map")
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), BossLevel);
}

void AIngameGameMode::OnMonsterSpawned()
{
    if (!IsValid(IngameState))
    {
        return;
    }
    IngameState->SetDoorVisibility(false);
    IngameState->IncreaseMosnterCount();
}

void AIngameGameMode::OnMonsterDead()
{
    if (!IsValid(IngameState))
    {
        return;
    }
    IngameState->DecreaseMosnterCount();
    if (IngameState->GetMonsterCount() <= 0 )
    {
        IngameState->SetDoorVisibility(true);
    }
}

void AIngameGameMode::OnBossDead()
{
    JLog("Boss Dead, Try to complete stage");

    IngameState->TryCompleteStage(GetWorld()->GetTimeSeconds() - IngameState->GetStartTime());
}

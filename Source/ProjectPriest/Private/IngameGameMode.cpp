#include "IngameGameMode.h"
#include "IngameGameState.h"
#include "JUtility.h"

void AIngameGameMode::BeginPlay()
{
    JASSERT(IsValid(GameState), "Game mode is not valid");

    IngameState = Cast<AIngameGameState>(GameState);
    JASSERT(IsValid(IngameState), "Game stae is not AIngameGameState");

    IngameState->SetStartTime(GetWorld()->TimeSeconds);
}

void AIngameGameMode::OnPlayerDead()
{
    JError("플레이어 사망 화면을 구현하세요");

    // 사망 상태 처리
    AIngameGameState* IngameGameState = GetWorld() ? GetWorld()->GetGameState<AIngameGameState>() : nullptr;
    if (IngameGameState)
    {
        IngameGameState->OnGameOver();
    }
}

void AIngameGameMode::OnOpenBossRoomDoor()
{
    JASSERT(IsValid(IngameState), "Game state is not valid");

    float StartTime = IngameState->GetStartTime();
    float NowTime = GetWorld()->GetTimeSeconds();
    float ElapsedTime = NowTime - StartTime;

    JError("클리어 화면을 구현하세요 클리어 시간은 %f", ElapsedTime);
}

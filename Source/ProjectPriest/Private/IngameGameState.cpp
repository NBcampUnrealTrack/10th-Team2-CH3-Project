#include "IngameGameState.h"
#include "IngamePlayerController.h"

AIngameGameState::AIngameGameState()
{
}

AIngameGameState::~AIngameGameState()
{
}

float AIngameGameState::GetStartTime()
{
    return StartTime;
}
float AIngameGameState::GetElapsedTime()
{
    return ElapsedTime;
}

void AIngameGameState::SetStartTime(float NewStartTime)
{
    StartTime = NewStartTime;
}

void AIngameGameState::SetElapsedTime(float NewElapsedTime)
{
    ElapsedTime = NewElapsedTime;
}       

void AIngameGameState::OnGameOver()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    AIngamePlayerController* IngamePlayerController = Cast<AIngamePlayerController>(PlayerController);

    if (!IngamePlayerController)
    {
        return;
    }

    IngamePlayerController->SetPause(true);

    //게임 오버 HUD 표시
    //ex) IngamePlayerController->ShowGameOverWidget();
}
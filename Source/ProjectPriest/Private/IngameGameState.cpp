#include "IngameGameState.h"

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

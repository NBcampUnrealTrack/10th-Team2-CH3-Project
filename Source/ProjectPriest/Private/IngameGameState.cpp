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

float AIngameGameState::SetStartTime(float StartTime)
{
    StartTime = StartTime;
}

float AIngameGameState::SetElapsedTime(float ElapsedTime)
{
    ElapsedTime = ElapsedTime;
}       

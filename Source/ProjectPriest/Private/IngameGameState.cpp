#include "IngameGameState.h"
#include "JUtility.h"

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
int AIngameGameState::GetMonsterCount()
{
    return MonsterCount;
}

FOnDoorVisibiliyChangedDelegate& AIngameGameState::GetOnDoorVisibiliyChangedDelegate()
{
    return OnDoorVisibiliyChangedDelegate;
}

void AIngameGameState::SetStartTime(float NewStartTime)
{
    StartTime = NewStartTime;
}

void AIngameGameState::SetElapsedTime(float NewElapsedTime)
{
    ElapsedTime = NewElapsedTime;
}

void AIngameGameState::SetMonsterCount(int NewCount)
{
    MonsterCount = NewCount;
}

void AIngameGameState::SetDoorVisibility(bool Visibility)
{
    if (DoorVisibility != Visibility )
    {
        DoorVisibility = Visibility;
        OnDoorVisibiliyChangedDelegate.Broadcast(DoorVisibility);
    }    
}

void AIngameGameState::IncreaseMosnterCount()
{
    MonsterCount++;
    JLog("IncreaseSpawnedMosnterCount %d", MonsterCount);
}

void AIngameGameState::DecreaseMosnterCount()
{
    MonsterCount--;
    JLog("DecreaseSpawnedMosnterCount %d", MonsterCount);    
}

#include "IngameGameState.h"
#include "JUtility.h"
#include "MvcControl.h"

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
int AIngameGameState::GetMonsterCount() const
{
    return MonsterCount;
}

FOnDoorVisibiliyChangedDelegate& AIngameGameState::GetOnDoorVisibiliyChangedDelegate()
{
    return OnDoorVisibiliyChangedDelegate;
}

void AIngameGameState::SetStartTime(float NewStartTime)
{
    if (bStageCleared)
    {
        return;
    }
    StartTime = NewStartTime;
}

void AIngameGameState::SetElapsedTime(float NewElapsedTime)
{
    if (bStageCleared)
    {
        return;
    }
    ElapsedTime = NewElapsedTime;
}

void AIngameGameState::SetMonsterCount(int NewCount)
{
    if (bStageCleared)
    {
        return;
    }
    const int ClampedCount = FMath::Max(0, NewCount);
    if (MonsterCount == ClampedCount)
    {
        return;
    }
    MonsterCount = ClampedCount;
    InvokePropertyChanged(0);
}

void AIngameGameState::SetDoorVisibility(bool Visibility)
{
    if (bStageCleared)
    {
        return;
    }
    if (DoorVisibility != Visibility )
    {
        DoorVisibility = Visibility;
        OnDoorVisibiliyChangedDelegate.Broadcast(DoorVisibility);
        InvokePropertyChanged(0);
    }    
}

void AIngameGameState::IncreaseMosnterCount()
{
    SetMonsterCount(MonsterCount + 1);
    JLog("IncreaseSpawnedMosnterCount %d", MonsterCount);
}

void AIngameGameState::DecreaseMosnterCount()
{
    SetMonsterCount(MonsterCount - 1);
    JLog("DecreaseSpawnedMosnterCount %d", MonsterCount);    
}

FDelegateHandle AIngameGameState::AddListener(UMvcControl* Control)
{
    return OnMissionChanged.AddUObject(Control, &UMvcControl::HandleModelChanged);
}
void AIngameGameState::RemoveListener(FDelegateHandle Handle) { OnMissionChanged.Remove(Handle); }
void AIngameGameState::InvokePropertyChanged(uint8 PropertyName) { OnMissionChanged.Broadcast(this, PropertyName); }

bool AIngameGameState::TryCompleteStage(float ClearSeconds)
{
    if (bStageCleared || MonsterCount != 0 || !DoorVisibility
        || !FMath::IsFinite(ClearSeconds) || ClearSeconds < 0.0f)
    {
        return false;
    }
    ElapsedTime = ClearSeconds;
    bStageCleared = true;
    DoorVisibility = false;
    OnDoorVisibiliyChangedDelegate.Broadcast(false);
    InvokePropertyChanged(0);
    return true;
}

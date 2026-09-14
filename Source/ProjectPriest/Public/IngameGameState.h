#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h" // FIX: 올바른 헤더 포함
#include "IngameGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDoorVisibiliyChangedDelegate, bool);

UCLASS()
class PROJECTPRIEST_API AIngameGameState : public AGameStateBase
{
    GENERATED_BODY()
public:  
    
	AIngameGameState();
	~AIngameGameState();

    float GetStartTime();
    float GetElapsedTime();
    int GetMonsterCount();
    FOnDoorVisibiliyChangedDelegate& GetOnDoorVisibiliyChangedDelegate();
    
    void SetStartTime(float NewStartTime);
    void SetElapsedTime(float NewElapsedTime);
    
    void OnGameOver();

    void SetMonsterCount(int NewCount);
    void SetDoorVisibility(bool Visibility);
    
    void IncreaseMosnterCount();
    void DecreaseMosnterCount();
    
protected:
    float StartTime;
    float ElapsedTime;
    int MonsterCount;
    bool DoorVisibility;
    FOnDoorVisibiliyChangedDelegate OnDoorVisibiliyChangedDelegate;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h" // FIX: 올바른 헤더 포함
#include "MvcModel.h"
#include "IngameGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDoorVisibiliyChangedDelegate, bool);

UCLASS()
class PROJECTPRIEST_API AIngameGameState : public AGameStateBase, public IMvcModel
{
    GENERATED_BODY()
public:  
    
	AIngameGameState();
	~AIngameGameState();

    float GetStartTime();
    float GetElapsedTime();
    bool HasStageCleared() const { return bStageCleared; }
    bool TryCompleteStage(float ClearSeconds);
    int GetMonsterCount() const;
    bool IsExitAvailable() const { return DoorVisibility; }
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokePropertyChanged(uint8 PropertyName) override;
    FOnDoorVisibiliyChangedDelegate& GetOnDoorVisibiliyChangedDelegate();
    
    void SetStartTime(float NewStartTime);
    void SetElapsedTime(float NewElapsedTime);
    
    void OnGameOver();

    void SetMonsterCount(int NewCount);
    void SetDoorVisibility(bool Visibility);
    
    void IncreaseMosnterCount();
    void DecreaseMosnterCount();
    
protected:
    float StartTime = 0.0f;
    float ElapsedTime = 0.0f;
    int MonsterCount = 0;
    bool DoorVisibility = false;
    bool bStageCleared = false;
    FModelChangedDelegate OnMissionChanged;
    FOnDoorVisibiliyChangedDelegate OnDoorVisibiliyChangedDelegate;
};

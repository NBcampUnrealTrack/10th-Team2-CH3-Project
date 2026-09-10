#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h" // FIX: 올바른 헤더 포함
#include "IngameGameState.generated.h"

UCLASS()
class PROJECTPRIEST_API AIngameGameState : public AGameStateBase
{
    GENERATED_BODY()
public:
	AIngameGameState();
	~AIngameGameState();

    float GetStartTime();
    float GetElapsedTime();

    void SetStartTime(float NewStartTime);
    void SetElapsedTime(float NewElapsedTime);

protected:
    float StartTime;
    float ElapsedTime;
};

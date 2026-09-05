#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h" // FIX: 올바른 헤더 포함

class PROJECTPRIEST_API AIngameGameState : public AGameStateBase
{
public:
	AIngameGameState();
	~AIngameGameState();

    float GetStartTime();
    float GetElapsedTime();

    float SetStartTime(float StartTime);
    float SetElapsedTime(float ElapsedTime);

protected:
    float StartTime;
    float ElapsedTime;
};

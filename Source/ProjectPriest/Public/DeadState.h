#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "DeadState.generated.h"

UCLASS()
class PROJECTPRIEST_API UDeadState : public UState
{
    GENERATED_BODY()

public:
    virtual void OnEnter() override;
    virtual void OnTick(float DeltaSeconds) override;
    virtual void OnExit() override;
};
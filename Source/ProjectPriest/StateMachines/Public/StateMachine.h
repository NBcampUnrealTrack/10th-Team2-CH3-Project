#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefineStateMachineDelegates.h"
#include "StateMachine.generated.h"

class UState;
class UTransition;

UCLASS()
class PROJECTPRIEST_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:

public:
    void OnEnter();
    void OnTick(float DeltaTime);
    void OnFinish();

    void AddState(TObjectPtr<UState> InNewState);
    void AddTransition(TObjectPtr<UState> InFromState, FTransitionCheckingDelegate, TObjectPtr<UState> InToState);
    
protected:
    void ChangeState(TObjectPtr<UState> InNextState);

protected:
    TArray<TObjectPtr<UState>> States;
    TMultiMap<TObjectPtr<UState>, TObjectPtr<UTransition>> TransitionMap;   
    TObjectPtr<UState> StartState;
    TObjectPtr<UState> CurrentState;
};

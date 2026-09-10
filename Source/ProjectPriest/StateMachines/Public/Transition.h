#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefineStateMachineDelegates.h"
#include "Transition.generated.h"

class UState;
class UStateMachine;
class UTransition;

UCLASS()
class PROJECTPRIEST_API UTransition : public UObject
{
	GENERATED_BODY()
	
public:
    static TObjectPtr<UTransition> Create(TObjectPtr<UStateMachine> Owner
        , TObjectPtr<UState> From
        , TObjectPtr<UState> To
        , FTransitionCheckingDelegate Delegate);

    bool CanTranstition();
    TObjectPtr<UState> GetFromState();
    TObjectPtr<UState> GetToState();

protected:
    TObjectPtr<UState> From;
    TObjectPtr<UState> To;
    FTransitionCheckingDelegate TransitionCallback;
};
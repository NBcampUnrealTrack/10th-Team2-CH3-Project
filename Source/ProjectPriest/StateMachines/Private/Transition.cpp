#include "Transition.h"
#include "State.h"
#include "StateMachine.h"

TObjectPtr<UTransition> UTransition::Create(
    TObjectPtr<UStateMachine> InOwner
    , TObjectPtr<UState> InFrom
    , TObjectPtr<UState> InTo
    , FTransitionCheckingDelegate InDelegate)
{
    FString TempName = FString::Format(
        TEXT("{0}_To_{1}"),
        {
            InFrom->GetDisplayName(),
            InTo->GetDisplayName()
        }
    );

    FName TransitionName(*TempName);
    TObjectPtr<UTransition> NewTransition = NewObject<UTransition>(InOwner, TransitionName);

    NewTransition->From = InFrom;
    NewTransition->To = InTo;
    NewTransition->TransitionCallback = InDelegate;

    return NewTransition;
}

bool UTransition::CanTranstition()
{
    return TransitionCallback.Execute(From, To);
}

TObjectPtr<UState> UTransition::GetFromState()
{
    return From;
}

TObjectPtr<UState> UTransition::GetToState()
{
    return To;
}

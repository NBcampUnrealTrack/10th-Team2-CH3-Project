#include "StateMachine.h"
#include "State.h"
#include "Transition.h"
#include "JUtility.h"

void UStateMachine::OnEnter()
{
    JASSERT(IsValid(StartState), "StartState is not exist");

    CurrentState = StartState;
    CurrentState->OnEnter();
}

void UStateMachine::OnTick(float DeltaSeconds)
{
    JASSERT(IsValid(CurrentState), "Current State is invalid");

    CurrentState->OnTick(DeltaSeconds);

    TArray<TObjectPtr<UTransition>> FoundMulti;

    TransitionMap.MultiFind(CurrentState, FoundMulti, true);

    for (UTransition* Transition : FoundMulti)
    {
        if (Transition->CanTranstition())
        {
            ChangeState(Transition->GetToState());
            break;
        }
    }
}

void UStateMachine::OnFinish()
{
    JASSERT(IsValid(CurrentState), "Current State is Invalid");

    CurrentState->OnExit();
}

void UStateMachine::AddState(TObjectPtr<UState> InNewState)
{
    States.Emplace(InNewState);
}

void UStateMachine::AddTransition(TObjectPtr< UState> InFromState, FTransitionCheckingDelegate InTranstitionCallback, TObjectPtr<UState> InToState)
{
    TObjectPtr<UTransition> NewTransition = UTransition::Create(this, InFromState, InToState, InTranstitionCallback);
    TransitionMap.Emplace(InFromState, NewTransition);
}

void UStateMachine::ChangeState(TObjectPtr<UState> InNextState)
{
    JASSERT(IsValid(CurrentState), "Current State is invalid");
    JASSERT(IsValid(InNextState), "Next state is invalid");

    CurrentState->OnExit();
    CurrentState = InNextState;
    CurrentState->OnEnter();
}

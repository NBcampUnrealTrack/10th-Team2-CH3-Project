#include "State.h"
#include "JUtility.h"

void UState::OnEnter()
{
    JLog("%s Entered", *DisplayName);
}

void UState::OnTick(float DeltaSeconds)
{
}

void UState::OnExit()
{
    JLog("%s Exit", *DisplayName);
}

const FString& UState::GetDisplayName() const
{
    return DisplayName;
}

void UState::SetDisplayName(const FString& NewDisplayName)
{
    DisplayName = NewDisplayName;
}
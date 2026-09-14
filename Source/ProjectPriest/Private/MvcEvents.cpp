#include "MvcEvents.h"

UButtonClickedEvent::UButtonClickedEvent()
{
}

EViewEventType UEventParameterBase::GetEventType()
{
    return EventType;
}

UButtonClickedEvent* UButtonClickedEvent::Create(EViewEventType EventType, EButtonName ButtonName)
{
    UButtonClickedEvent* NewClickedEvent = NewObject<UButtonClickedEvent>();

    NewClickedEvent->EventType = EViewEventType::ButtonClicked;
    NewClickedEvent->ButtonName = ButtonName;

    return NewClickedEvent;
}
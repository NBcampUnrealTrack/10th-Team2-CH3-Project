#pragma once

/*
 * START VIEW UITILITY MACRO FUNCTION SECTIONS
 */
#define DECLARE_VIEW_DEFAULT_INTERFACES() \
public:\
    virtual FDelegateHandle AddListener(UMvcControl* Control) override; \
    virtual void RemoveListener(FDelegateHandle DelegateHandle) override; \
    virtual void InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter)override; \
protected:\
    virtual void NativeOnInitialized() override;\
protected:\
    UPROPERTY() \
    TObjectPtr<UMvcControl> MvcControl;\
    FViewEventRaisedDelegate ViewEventListeners;

#define IMPLEMENT_VIEW_DEFAULT_INTERFACE(ViewClassName, ControllerClassName) \
IMPLEMENT_VIEW_DEFAULT_ADDLISTENER(ViewClassName) \
IMPLEMENT_VIEW_DEFAULT_REMOVELISTENER(ViewClassName) \
IMPLEMENT_VIEW_DEFAULT_INVOKE_VIEW_EVENT(ViewClassName) \
IMPLEMENT_VIEW_DEFAULT_INIT(ViewClassName, ControllerClassName)

#define IMPLEMENT_VIEW_DEFAULT_ADDLISTENER(ViewClassName) \
FDelegateHandle ViewClassName::AddListener(UMvcControl* Control) \
{ \
	return ViewEventListeners.AddUObject(Control, &UMvcControl::HandleViewEvent); \
} \

#define IMPLEMENT_VIEW_DEFAULT_REMOVELISTENER(ViewClassName) \
void ViewClassName::RemoveListener(FDelegateHandle DelegateHandle) \
{ \
	ViewEventListeners.Remove(DelegateHandle); \
} \

#define IMPLEMENT_VIEW_DEFAULT_INVOKE_VIEW_EVENT(ViewClassName) \
void ViewClassName::InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter) \
{ \
	ViewEventListeners.Broadcast(this, EventName, Parameter); \
} \

#define IMPLEMENT_VIEW_DEFAULT_INIT(ViewClassName, ControllerClassName) \
void ViewClassName::NativeOnInitialized() \
{ \
	Super::NativeOnInitialized(); \
	MvcControl = ControllerClassName::Create(this, GetWorld()); \
	MvcControl->SetView(this); \
}
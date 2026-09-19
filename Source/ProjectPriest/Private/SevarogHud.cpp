#include "SevarogHud.h"
#include "SevarogHudController.h"
#include "JUtility.h" 
#include "Components/ProgressBar.h"

void USevarogHud::SetHealthRate(float Rate)
{
	JASSERT(IsValid(HealthProgressBar),"is not a valid HealthProgressBar");
	
	HealthProgressBar->SetPercent(Rate);
}

FDelegateHandle USevarogHud::AddListener(UMvcControl* Control)
{
	return ViewEventListeners.AddUObject(Control, &UMvcControl::HandleViewEvent);
}

void USevarogHud::RemoveListener(FDelegateHandle DelegateHandle)
{
	ViewEventListeners.Remove(DelegateHandle);
}

void USevarogHud::InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter)
{
	//has noting to raise event
}

void USevarogHud::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	MvcControl = USevarogHudController::Create(this, GetWorld());
	
	MvcControl->SetView(this);
}

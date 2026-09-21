#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "MvcEvents.h"

#include "SevarogHud.generated.h"

class UProgressBar;
class UMvcControl;

UCLASS()
class PROJECTPRIEST_API USevarogHud : public UUserWidget, public IMvcView
{
	GENERATED_BODY()

public:
    void SetHealthRate(float Rate);

	virtual FDelegateHandle AddListener(UMvcControl* Control) override;
	virtual void RemoveListener(FDelegateHandle DelegateHandle) override;
	virtual void InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter) override;
	
protected:
	virtual void NativeOnInitialized() override;
	
protected:
	UPROPERTY()
	TObjectPtr<UMvcControl> MvcControl;
	
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthProgressBar;	
	
	FViewEventRaisedDelegate ViewEventListeners;
};

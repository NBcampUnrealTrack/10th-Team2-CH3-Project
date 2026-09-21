#pragma once

#include "CoreMinimal.h"
#include "MvcControl.h"
#include "AlertIndicatorControl.generated.h"

UCLASS()
class PROJECTPRIEST_API UAlertIndicatorControl : public UMvcControl
{
	GENERATED_BODY()

public:
	virtual void HandleViewEvent(IMvcView* InView
		, EViewEventType EventType
		, UEventParameterBase* Parameter) override;
	
	virtual void HandleModelChanged(IMvcModel* InModel
		, uint8 PropertyName) override;	
	
	static UAlertIndicatorControl* Create(IMvcView* View, UWorld* World);
};

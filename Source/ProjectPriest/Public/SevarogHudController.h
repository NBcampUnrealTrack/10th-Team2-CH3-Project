#pragma once

#include "CoreMinimal.h"
#include "MvcControl.h"
#include "SevarogHudController.generated.h"

class IMvcView;

UCLASS()
class PROJECTPRIEST_API USevarogHudController : public UMvcControl
{
	GENERATED_BODY()
	
	virtual void HandleViewEvent(IMvcView* InView
		, EViewEventType EventType
		, UEventParameterBase* Parameter) override;
	
	virtual void HandleModelChanged(IMvcModel* InModel
		, uint8 PropertyName) override;
public:
	static USevarogHudController* Create(IMvcView* View, UWorld* World);
};

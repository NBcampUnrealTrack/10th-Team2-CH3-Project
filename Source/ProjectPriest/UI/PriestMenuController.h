#pragma once

#include "CoreMinimal.h"
#include "MvcControl.h"
#include "PriestMenuController.generated.h"

UCLASS()
class PROJECTPRIEST_API UPriestMenuController : public UMvcControl
{
	GENERATED_BODY()

public:
	virtual void HandleViewEvent(
		IMvcView* InView,
		EViewEventType EventType,
		UEventParameterBase* Parameter
	) override;

	virtual void HandleModelChanged(
		IMvcModel* InModel,
		uint8 PropertyName
	) override;
};

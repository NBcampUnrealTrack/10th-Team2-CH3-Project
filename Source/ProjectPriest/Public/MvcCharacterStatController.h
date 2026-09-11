#pragma once

#include "CoreMinimal.h"
#include "MvcControl.h"
#include "MvcCharacterStatController.generated.h"

UCLASS()
class PROJECTPRIEST_API UMvcCharacterStatController : public UMvcControl
{
    GENERATED_BODY()
public:
	UMvcCharacterStatController();

    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) override;

    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) override;
};

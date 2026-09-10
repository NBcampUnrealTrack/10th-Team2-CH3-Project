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

    virtual void HandleViewEvent(IMvcView* View, EViewEventType EventType, UEventParameterBase* Parameter) override;

    virtual void HandleModelChanged(IMvcModel* Model, uint8 PropertyName) override;
};

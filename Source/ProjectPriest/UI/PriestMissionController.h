#pragma once
#include "CoreMinimal.h"
#include "MvcControl.h"
#include "PriestMissionController.generated.h"

UCLASS()
class PROJECTPRIEST_API UPriestMissionController : public UMvcControl
{
    GENERATED_BODY()
public:
    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) override;
    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) override {}
    FText GetMissionText() const;
};

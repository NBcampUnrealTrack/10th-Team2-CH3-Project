#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MvcEvents.h"
#include "MvcControl.generated.h"

class IMvcModel;
class IMvcView;

template<class T>
concept DrivedMvcView = std::derived_from<T, IMvcView>;

template<class T>
concept DrivedMvcModel = std::derived_from<T, IMvcModel>;

UCLASS()
class PROJECTPRIEST_API UMvcControl : public UObject
{
    GENERATED_BODY()

public:
    UMvcControl();

    virtual void BeginDestroy() override;

    void SetView(IMvcView* View);
    void SetModel(IMvcModel* Model);

    template<DrivedMvcView T>
    T* GetView();
    
    template<DrivedMvcModel T>
    T* GetModel();

    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) PURE_VIRTUAL(&UMvcControl::HandleViewEvent, return;) ;

    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) PURE_VIRTUAL(&UMvcControl::HandleModelChanged, return;) ;


protected:
    FDelegateHandle ViewListenerHandle;
    FDelegateHandle ModelListenerHandle;

    IMvcModel* Model;
    IMvcView* View;
};
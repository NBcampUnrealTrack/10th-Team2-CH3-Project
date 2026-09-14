#pragma once
#include "CoreMinimal.h"
#include "MvcEvents.h"
#include "MvcControl.generated.h"
class IMvcModel;
class IMvcView;
UCLASS()
class PROJECTPRIEST_API UMvcControl : public UObject
{
    GENERATED_BODY()
public:
    virtual void BeginDestroy() override;
    void SetView(IMvcView* View);
    void SetModel(IMvcModel* Model);
    void Disconnect();
    template<class T> T* GetView() { return Cast<T>(ViewObject.Get()); }
    template<class T> T* GetModel() { return Cast<T>(ModelObject.Get()); }
    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) PURE_VIRTUAL(UMvcControl::HandleViewEvent, return;);
    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) PURE_VIRTUAL(UMvcControl::HandleModelChanged, return;);
protected:
    TWeakObjectPtr<UObject> ModelObject;
    TWeakObjectPtr<UObject> ViewObject;
    FDelegateHandle ViewListenerHandle;
    FDelegateHandle ModelListenerHandle;
};

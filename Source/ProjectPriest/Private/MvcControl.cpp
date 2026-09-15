#include "MvcControl.h"
#include "MvcModel.h"
#include "MvcView.h"

void UMvcControl::BeginDestroy()
{
    Disconnect();
    Super::BeginDestroy();
}

void UMvcControl::Disconnect()
{
    SetView(nullptr);
    SetModel(nullptr);
}

void UMvcControl::SetView(IMvcView* NewView)
{
    // 이전 뷰가 살아 있을 때만 구독을 해제한다.
    if (IMvcView* Previous = Cast<IMvcView>(ViewObject.Get()))
    {
        Previous->RemoveListener(ViewListenerHandle);
    }
    ViewListenerHandle.Reset();
    ViewObject = NewView ? NewView->_getUObject() : nullptr;
    if (NewView && ViewObject.IsValid())
    {
        ViewListenerHandle = NewView->AddListener(this);
    }
}

void UMvcControl::SetModel(IMvcModel* NewModel)
{
    if (IMvcModel* Previous = Cast<IMvcModel>(ModelObject.Get()))
    {
        Previous->RemoveListener(ModelListenerHandle);
    }
    ModelListenerHandle.Reset();
    ModelObject = NewModel ? NewModel->_getUObject() : nullptr;
    if (NewModel && ModelObject.IsValid())
    {
        ModelListenerHandle = NewModel->AddListener(this);
    }
}

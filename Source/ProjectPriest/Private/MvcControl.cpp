#include "MvcControl.h"
#include "MvcModel.h"
#include "MvcView.h"

UMvcControl::UMvcControl()
{
}

void UMvcControl::BeginDestroy()
{
    Model->RemoveListener(ModelListenerHandle);
    View->RemoveListener(ViewListenerHandle);
}

void UMvcControl::SetView(IMvcView* NewView)
{
    View = NewView;
    ViewListenerHandle = View->AddListener(this);
}

template<DrivedMvcView T>
T* UMvcControl::GetView()
{
    return View;
}

void UMvcControl::SetModel(IMvcModel* NewModel)
{
    Model = NewModel;
    ModelListenerHandle = Model->AddListener(this);
}

template<DrivedMvcModel T>
T* UMvcControl::GetModel()
{
    return Model;
}

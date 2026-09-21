#include "PriestMenuModel.h"

#include "MvcControl.h"

void UPriestMenuModel::SetState(EPriestMenuPage NewPage, EPriestLobbyTab NewTab)
{
	Page = NewPage;
	Tab = NewTab;
	InvokePropertyChanged(0);
}

FDelegateHandle UPriestMenuModel::AddListener(UMvcControl* Control)
{
	return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}

void UPriestMenuModel::RemoveListener(FDelegateHandle Handle)
{
	Changed.Remove(Handle);
}

void UPriestMenuModel::InvokePropertyChanged(uint8 PropertyName)
{
	Changed.Broadcast(this, PropertyName);
}

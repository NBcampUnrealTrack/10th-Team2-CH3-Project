#include "MaterialItem.h"
#include "MaterialTable.h"
#include "Engine/DataTable.h"
// #include "InventoryComponent.h"

AMaterialItem::AMaterialItem()
{
	ItemType = TEXT("Material");
}

void AMaterialItem::ActivateItem(AActor* Activator)
{
	if (!Activator)
	{
		return;
	}

	/*UInventoryComponent* Inventory = Activator->FindComponentByClass<UInventoryComponent>();

	if (!Inventory)
	{
		return;
	}

	Inventory->AddItem(MaterialID, Quantity); */

	DestroyItem();
}

const FMaterialTable* AMaterialItem::GetMaterialData() const
{
	if (!MaterialDataTable || MaterialID.IsNone())
	{
		return nullptr;
	}

	return MaterialDataTable->FindRow<FMaterialTable>(MaterialID, TEXT("MaterialItem"));
}
#include "MaterialItem.h"
#include "MaterialTable.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.h"
#include "JUtility.h"

AMaterialItem::AMaterialItem()
{
	ItemType = TEXT("Material");
}

void AMaterialItem::ActivateItem(AActor* Activator)
{
	JASSERT(Activator, "Activator가 없습니다.");

	UInventoryComponent* Inventory = Activator->FindComponentByClass<UInventoryComponent>();

	JASSERT(Inventory, "Inventory가 없습니다.");

	Inventory->AddItem(MaterialID, Quantity);

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
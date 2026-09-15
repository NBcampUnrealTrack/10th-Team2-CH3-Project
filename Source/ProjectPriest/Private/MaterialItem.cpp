#include "MaterialItem.h"

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

	// 인벤토리에 재료 추가
	// ex) Inventory.quantity += quantity;

	DestroyItem();
}
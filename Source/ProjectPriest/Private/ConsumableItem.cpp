#include "ConsumableItem.h"

AConsumableItem::AConsumableItem()
{
	ItemType = TEXT("Consumable");
}

void AConsumableItem::ActivateItem(AActor* Activator)
{
	if (!Activator)
	{
		return;
	}

	// 소모 아이템 사용 효과는 자식 클래스에서 구현
	// ex) 회복 포션, 버프 아이템 등

	DestroyItem();
}
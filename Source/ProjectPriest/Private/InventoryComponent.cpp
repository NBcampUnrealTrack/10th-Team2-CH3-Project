#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
	if (ItemID.IsNone() || Quantity <= 0)
	{
		return false;
	}

	// 동일한 아이템이 인벤토리에 존재할 때 수량을 증가
	for (FInventoryItem& Item : Items)
	{
		if (Item.ItemID == ItemID)
		{
			Item.Quantity += Quantity;
			return true;
		}
	}

	// 동일한 아이템이 인벤토리에 존재하지 않을 때 인벤토리에 새로운 아이템 생성
	FInventoryItem NewItem;
	NewItem.ItemID = ItemID;
	NewItem.Quantity = Quantity;

	Items.Add(NewItem);

	return true;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
	if (ItemID.IsNone() || Quantity <= 0)
	{
		return false;
	}

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == ItemID)
		{
			// 인벤토리에서 뺄 양이 가진 것보다 많으면 false
			if (Items[i].Quantity < Quantity)
			{
				return false;
			}

			Items[i].Quantity -= Quantity;

			// 인벤토리에 남은 수량이 0이면 인벤토리에서 아이템 제거
			if (Items[i].Quantity <= 0)
			{
				Items.RemoveAt(i);
			}

			return true;
		}
	}

	return false;
}

int32 UInventoryComponent::GetItemQuantity(FName ItemID) const
{
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemID == ItemID)
		{
			return Item.Quantity;
		}
	}

	return 0;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
	// Quantity에는 보통 1이 들어감
	return GetItemQuantity(ItemID) >= Quantity;
}
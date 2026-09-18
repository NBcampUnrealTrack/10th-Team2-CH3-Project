#include "InventoryComponent.h"
#include "Engine/DataTable.h"
#include "JUtility.h"

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
		if (Item.ItemID.IsNone())
		{
			JError("Inventory에 ItemID가 None인 아이템이 존재합니다.");
			return false;
		}

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
		JError("유효하지 않은 삭제입니다.");
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

bool UInventoryComponent::CraftItem(FName RecipeID)
{
	JASSERT(RecipeDataTable, "RecipeDataTable이 없습니다.");

	const FRecipeData* RecipeData = RecipeDataTable->FindRow<FRecipeData>(RecipeID, TEXT("UInventoryComponent::CraftItem"));

	JASSERT(RecipeData, "Recipe를 찾을 수 없습니다.");

	// 재료 보유 여부 확인
	for (const FRecipeIngredient& Ingredient : RecipeData->Ingredients)
	{
		if (!HasItem(Ingredient.ItemID, Ingredient.Quantity))
		{
			return false;
		}
	}

	// 인벤토리에서 재료 감소
	for (const FRecipeIngredient& Ingredient : RecipeData->Ingredients)
	{
		RemoveItem(Ingredient.ItemID, Ingredient.Quantity);
	}

	// 인벤토리에 바로 제작 아이템 추가
	AddItem(RecipeData->ResultItemID, RecipeData->ResultQuantity);

	return true;
}
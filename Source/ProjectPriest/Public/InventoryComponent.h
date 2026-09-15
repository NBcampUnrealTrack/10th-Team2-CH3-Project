#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTPRIEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Inventory===|Items")
	TArray<FInventoryItem> Items;
		
public:
	// 인벤토리에 아이템을 넣는 함수
	bool AddItem(FName ItemID, int32 Quantity);

	// 인벤토리에서 아이템을 제거하는 함수
	bool RemoveItem(FName ItemID, int32 Quantity);

	// 아이템 수량 반환 함수
	int32 GetItemQuantity(FName ItemID) const;

	// 아이템 수량만큼 보유 여부 반환 함수, 아이템 제작에 사용
	bool HasItem(FName ItemID, int32 Quantity) const;
};
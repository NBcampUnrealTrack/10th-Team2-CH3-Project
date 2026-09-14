#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ConsumableItem.generated.h"

UCLASS()
class PROJECTPRIEST_API AConsumableItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AConsumableItem();

protected:
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Consumable===|Properties")
	int32 Quantity = 1;

protected:
	// Internal Functions
	virtual void ActivateItem(AActor* Activator) override;
};

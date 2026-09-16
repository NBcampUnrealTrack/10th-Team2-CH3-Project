#pragma once

#include "CoreMinimal.h"
#include "ConsumableItem.h"
#include "AmmoUpItem.generated.h"

UCLASS()
class PROJECTPRIEST_API AAmmoUpItem : public AConsumableItem
{
	GENERATED_BODY()
	
public:
	AAmmoUpItem();

protected:
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===AmmoUp===|Properties")
	int32 AmmoAmount = 10;

protected:
	// Internal Functions
	virtual void ActivateItem(AActor* Activator) override;
};
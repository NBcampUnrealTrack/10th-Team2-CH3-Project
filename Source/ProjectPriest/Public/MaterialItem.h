#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MaterialItem.generated.h"

class UDataTable;
struct FMaterialTable;

UCLASS()
class PROJECTPRIEST_API AMaterialItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMaterialItem();

protected:
	// Data Table
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "===Material===|Data")
	TObjectPtr<UDataTable> MaterialDataTable;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Material===|Properties")
	FName MaterialID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Material===|Properties")
	int32 Quantity = 1;

protected:
	// Internal Functions
	virtual void ActivateItem(AActor* Activator) override;

	const FMaterialTable* GetMaterialData() const;
};
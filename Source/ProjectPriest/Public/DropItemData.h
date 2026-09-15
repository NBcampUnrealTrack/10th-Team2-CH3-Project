#pragma once

#include "CoreMinimal.h"
#include "DropItemData.generated.h"

class ABaseItem;

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FDropItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseItem> ItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity = 1;
};
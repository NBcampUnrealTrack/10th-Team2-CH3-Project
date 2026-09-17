#pragma once

#include "CoreMinimal.h"
#include "DropItemData.generated.h"

class ABaseItem;

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FDropItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseItem> MaterialItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseItem> AmmoItemClass = nullptr;
};
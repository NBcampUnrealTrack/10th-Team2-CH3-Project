#pragma once

#include "CoreMinimal.h"
#include "DropItemData.generated.h"

class ABaseItem;

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FDropItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseItem>> MaterialItemClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseItem> AmmoItemClass = nullptr;
};
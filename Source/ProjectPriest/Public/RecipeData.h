#pragma once

#include "CoreMinimal.h"
#include "RecipeData.generated.h"

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FRecipeIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FRecipeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecipeIngredient> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ResultItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResultQuantity = 1;
};
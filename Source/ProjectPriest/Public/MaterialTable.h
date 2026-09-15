#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MaterialTable.generated.h"

USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FMaterialTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack = 99;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PartItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPRIEST_API APartItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	APartItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	FDataTableRowHandle PartData;
};

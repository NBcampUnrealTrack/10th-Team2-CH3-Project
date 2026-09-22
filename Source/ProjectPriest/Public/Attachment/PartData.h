// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartData.generated.h"

UENUM(BlueprintType)
enum class EPartSlot : uint8
{
	None,
	Barrel,
	Magazine
};

USTRUCT(BlueprintType)
struct FPartData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPartSlot SlotType = EPartSlot::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagazineSize = -1.0f;
};

USTRUCT(BlueprintType)
struct FPartSlotViewData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EPartSlot SlotType = EPartSlot::None;

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(BlueprintReadOnly)
	FText StatName;

	UPROPERTY(BlueprintReadOnly)
	FText StatValue;
};

USTRUCT(BlueprintType)
struct FPartViewData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FPartSlotViewData> Parts;
};
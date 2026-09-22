// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Attachment/PartData.h"
#include "PartSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FWeaponPartSlots
{
	GENERATED_BODY()

	UPROPERTY()
	FText WeaponName;
	//타입에 따른 파츠
	UPROPERTY()
	TMap<EPartSlot, FPartData> PartSlots;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPartsChanged);

//USTRUCT(BlueprintType)
//struct PROJECTPRIEST_API FPriestOwnedItem
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemId;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1")) int32 Quantity = 1;
//};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPriestQuickSlotsChanged);

UCLASS()
class PROJECTPRIEST_API UPartSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//FName -> FText -> WeponName
	FWeaponPartSlots* GetOrCreatePartInstance(FName WeaponName);

	//WeaponName으로 호출
	//WeaponName
	//Part 장착 파츠
	FPartData SetPart(FName WeaponName, FPartData& Part);

	//슬롯으로 호출
	//WeaponSlots -> 슬롯
	//Part 장착 파츠
	FPartData SetPart(FWeaponPartSlots* WeaponSlots, FPartData& Part);

	FPartData RemovePart(FName WeaponName, EPartSlot SlotType);
	
	//WeaponSlots -> 지울 슬롯
	//SlotType -> 지울 타입 
	FPartData RemovePart(FWeaponPartSlots* WeaponSlots, EPartSlot SlotType);
	
	FPartData GetEquippedPart(FName WeaponName, EPartSlot SlotType);

	//WeaponSlots -> 찾을 슬롯
	//SlotType -> 찾을 타입 
	FPartData GetEquippedPart(FWeaponPartSlots* WeaponSlots, EPartSlot SlotType);

	FWeaponPartSlots GetEquippedParts(FName WeaponName);

public:
	UPROPERTY(BlueprintAssignable, Category = "Priest|Inventory")
	FPartsChanged OnPartsChanged;

private:
	//FName -> FText -> WeponName
	//EPartSlot -> 파츠 슬롯
	//FPartData -> 파츠 구조체
	UPROPERTY()
	TMap<FName, FWeaponPartSlots> WeaponPartSlots;

};
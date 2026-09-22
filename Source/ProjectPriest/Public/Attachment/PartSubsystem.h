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

UCLASS()
class PROJECTPRIEST_API UPartSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//FName -> FText -> WeponName
	FWeaponPartSlots* GetOrCreatePartInstance(FName WeaponName);
	//개임 시작시 map에 데이터가 없다 -> 어차피 보유 파츠도 없구나 데이터가 없으면 UI표시는 어떤식으로 하지

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

private:
	//FName -> FText -> WeponName
	//EPartSlot -> 파츠 슬롯
	//FPartData -> 파츠 구조체
	UPROPERTY()
	TMap<FName, FWeaponPartSlots> WeaponPartSlots;

};
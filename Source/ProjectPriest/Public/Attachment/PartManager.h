// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PartData.h"
#include "PartManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPRIEST_API UPartManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPartManager();

	//파츠 장착
	//part -> 장착을 시도하는 파츠
	//ReplacedPart -> 장착 성공시 그 자리에 있던 파츠 반환
	void SetPart(const FPartData& Part, FPartData& ReplacedPart);

	//파츠 장착해제
	//Part -> 장착중 파츠 반환
	bool RemovePart(const FPartData& Part);

	//추후
	//데이터 인스턴스로 저장

	//인스턴스에서 불러오기 -> 적용까지 이어서?
private:
	//슬롯[배열] -> 인스턴스에 저장해야할 정보
	//맵의 키값으로 enum으로 미리 정해서 중복으로 못받도록 
	//무기마다 다른 슬롯을 구현하고싶으면 사용할 슬롯을 배열에 넣어 유효성 검사를 한번 더 하여 적용
	TMap<EPartSlot,FPartData>PartSlots;

	//무가에 적용 시켜줄 함수
	void ApplyPartsToWeapon(class AWeaponItem* Weapon);

	//변경 데이터의 기본값을 저장할 변수 -> 자료형이 다르니 따로
	float BaseDamage = -1.0f;

	int32 BaseMagazineSize = -1;
};

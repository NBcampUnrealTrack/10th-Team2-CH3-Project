// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PartData.h"
#include "Attachment/PartInstance.h"
#include "PartManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
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

	void SetPartInstance(UPartInstance* NewPartInstance);

	//무가에 적용 시켜줄 함수
	void ApplyPartsToWeapon(class AWeaponItem* Weapon);

	UFUNCTION(BlueprintCallable)
	FPartData GetPartSlot(EPartSlot SlotType) const;

private:
	//PartManager를 소유한 인스턴스
	UPROPERTY()
	TObjectPtr<UPartInstance> PartInstance;

	//변경 데이터의 기본값을 저장할 변수 -> 자료형이 다르니 따로
	float BaseDamage = -1.0f;

	int32 BaseMagazineSize = -1;
};

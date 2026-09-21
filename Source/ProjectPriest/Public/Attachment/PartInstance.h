// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PartData.h"
#include "PartInstance.generated.h"

class UPartManager;

UCLASS(BlueprintType)
class PROJECTPRIEST_API UPartInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UPartInstance();

	UFUNCTION(BlueprintCallable)
	UPartManager* GetPartManager() { return PartManager; }

	void Initialize();

	//슬롯[배열] -> 인스턴스에 저장해야할 정보
	//맵의 키값으로 enum으로 미리 정해서 중복으로 못받도록 
	//무기마다 다른 슬롯을 구현하고싶으면 사용할 슬롯을 배열에 넣어 검사를 한번 더 하여 적용

	UPROPERTY()
	TMap<EPartSlot, FPartData> PartSlots;

protected:
	UPROPERTY()
	TObjectPtr<UPartManager> PartManager;
};

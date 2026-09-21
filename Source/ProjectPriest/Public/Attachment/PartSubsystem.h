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

	//UI상 표기 이름 -> 따로 들어오지 않으면 키 값을 그대로 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WeaponName;
	//타입에 따른 파츠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EPartSlot, FPartData> PartSlots;
};

UCLASS()
class PROJECTPRIEST_API UPartSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//FName -> FText -> WeponName
	//EPartSlot -> 파츠 슬롯
	//FPartData -> 파츠 구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//WBP에서 변경 할일이 있을수도?
	TMap<FName, FWeaponPartSlots> WeaponPartSlots;
};

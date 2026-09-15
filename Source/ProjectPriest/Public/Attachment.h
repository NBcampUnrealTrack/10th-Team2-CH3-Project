// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attachment.generated.h"

class AWeaponItem;

UCLASS()
class PROJECTPRIEST_API UAttachment : public UObject
{
	GENERATED_BODY()
	
public:
	UAttachment();

protected:
	virtual void Equip(AWeaponItem* Weapon);
	virtual void Unequip(AWeaponItem* Weapon);
};

//파츠를 장착하는 칸을 별도의 class로 만들어서 사용 ? ->현재 재작 예정파츠가 2개인데 굳이 ?
//
//장착을 확인하는 방법
//ㄴ > 파츠를 레벨이 초기화가 되어도 사지지 않도록 조치가 필요
//ㄴ > 인벤토리 내부에서 파츠를 보유 하고 있으면서 파츠가 장착되었는지 여부를 확인 하여 인게임에서 스폰될 플레이어에게 파츠를 전달해야함
//
//맵에 숨어있는 파츠->플레이어가 가까이 붙음(획득) + (상호작용 키를 사용 할것인가)->인벤토리에 아이템 추가->아이템 장착시 총기 스팩 변경->아이템 해제시 총기 스팩 복구
//맵에 떨어져 있는 액터 제작
//ㄴ > 콜리전 추가하여 아이템 획득 혹은 상호작용 활성화
//ㄴ > 맵에있는 액터를 파괴 하면서 class정보를 인밴토리로 넘겨서 관리
//
//인벤토리 내부 class ?
//ㄴ > 실제 아이템에 부착 되었을때 스팩 증가 + 해제되었을때 복구의 로직을 구현->사실상 게임이 레벨마다 초기화 복구 로직이 의미가 있나 ?
//
//드래그엔 드랖으로 빼고 낄거여서 장착 및 해제 ? 가 가능하도록 설계->현재는 메뉴창 게임시작 전에만 변경 가능하도록 할 예정
//
//사거리나 정확도 등 현제 구현되어있지 않거나 적합하지 않아 추후 더 필요하면 논의
//
//총열->데미지
//
//확장탄창->탄창 용량
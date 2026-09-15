#pragma once

#include "CoreMinimal.h"
#include "PriestHUDData.generated.h"

// 게임 로직에서 UI로 전달하는 전체 표시 데이터. 체력 차감 등의 판정은 게임 로직이 담당한다.
// 갱신할 때 전체를 덮어쓰므로, 바꾸지 않는 항목도 현재 값을 유지해서 전달한다.
USTRUCT(BlueprintType)
struct PROJECTPRIEST_API FPriestHUDData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	float Health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	FText WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	int32 MagazineAmmo = 30; // 현재 탄창에 남은 탄약.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	int32 ReserveAmmo = 120; // 재장전에 사용할 보유 탄약.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	FText MissionObjective;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	float ElapsedSeconds = 0.0f; // 초 단위. 화면에는 숨기며, 누적 계산은 전달하는 쪽에서 한다.
};

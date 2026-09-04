#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PriestHUDData.h"
#include "PriestHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

// 전달받은 데이터를 표시하는 위젯. 별도 WBP 에셋 없이 C++로 화면을 구성한다.
UCLASS()
class PROJECTPRIEST_API UPriestHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 데이터를 보관한 후 표시를 갱신한다. 위젯 생성 전 호출되어도 데이터는 유지된다.
	void SetHUDData(const FPriestHUDData& InData);

protected:
	// 엔진이 화면 구성을 요청할 때 캔버스와 자식 위젯을 만든다.
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	// 보관한 데이터를 화면에 반영한다. 게임 상태 자체는 변경하지 않는다.
	void Refresh();
	UPROPERTY(Transient) FPriestHUDData Data;
	UPROPERTY(Transient) TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> HealthText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> WeaponText;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> MissionText;
};

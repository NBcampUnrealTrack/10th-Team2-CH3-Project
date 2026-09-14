#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PriestHUDData.h"
#include "PriestHUDTestActor.generated.h"

class UPriestHUDWidget;

// 테스트 맵에 하나 배치하고 Play하면 HUD를 확인할 수 있다.
// 매초 실제 HUD 데이터를 덮어쓰므로 게임 로직 연결 전에는 반드시 제거한다.
UCLASS()
class PROJECTPRIEST_API APriestHUDTestActor : public AActor
{
	GENERATED_BODY()
public:
	APriestHUDTestActor();
	// 테스트할 WBP. 비워두면 PlayerController에서 설정한 클래스를 사용한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Test")
	TSubclassOf<UPriestHUDWidget> HUDWidgetClass;
	// 에디터 디테일 패널에서 수정하는 초기 데이터.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Test") FPriestHUDData TestData;
	// 끄면 초기 값을 그대로 표시한다. 켜면 체력/탄약/시간을 테스트용으로 변화시킨다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Test") bool bAnimateData = true;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
private:
	void PushTestData();
	FTimerHandle TestTimer;
	int32 Step = 0;
};

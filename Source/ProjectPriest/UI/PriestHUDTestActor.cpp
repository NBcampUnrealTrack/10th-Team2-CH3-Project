#include "PriestHUDTestActor.h"
#include "PriestUIManager.h"
#include "PriestHUDWidget.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

APriestHUDTestActor::APriestHUDTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
	TestData.WeaponName = FText::FromString(TEXT("TEST RIFLE"));
	TestData.MissionObjective = FText::FromString(TEXT("TEST: Reach the checkpoint"));
}

void APriestHUDTestActor::BeginPlay()
{
	Super::BeginPlay();
	// 시작 즉시 표시하고 이후 1초 간격으로 갱신한다. 매 프레임 Tick은 사용하지 않는다.
	PushTestData();
	GetWorldTimerManager().SetTimer(TestTimer, this, &APriestHUDTestActor::PushTestData, 1.0f, true);
}

void APriestHUDTestActor::PushTestData()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	// 첫 번째 로컬 플레이어용 테스트다. 아직 준비되지 않았다면 다음 타이머 호출에서 재시도한다.
	ULocalPlayer* Player = PC ? PC->GetLocalPlayer() : nullptr;
	if (!Player) return;
	UPriestUIManager* UI = Player->GetSubsystem<UPriestUIManager>();
	if (UI && HUDWidgetClass) UI->SetHUDWidgetClass(HUDWidgetClass);
	FPriestHUDData Snapshot = TestData;
	// 원본 설정을 유지하려고 복사본만 변경한다. 실제 캐릭터의 체력/탄약에는 영향을 주지 않는다.
	if (bAnimateData)
	{
		Snapshot.Health = TestData.MaxHealth * (1.0f - (Step % 11) / 10.0f);
		Snapshot.MagazineAmmo = FMath::Max(0, TestData.MagazineAmmo - Step % 31);
		// 표시 성공 횟수로 만든 테스트 시간이며, 실제 게임의 정밀한 시간 측정 로직은 아니다.
		Snapshot.ElapsedSeconds = TestData.ElapsedSeconds + Step;
	}
	if (UI && UI->ShowHUD(Snapshot)) ++Step;
}

void APriestHUDTestActor::EndPlay(const EEndPlayReason::Type Reason)
{
	GetWorldTimerManager().ClearTimer(TestTimer);
	// 액터 종료 후에도 갱신 콜백이 남지 않도록 타이머를 해제한다.
	Super::EndPlay(Reason);
}

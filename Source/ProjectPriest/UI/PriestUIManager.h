#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "PriestHUDData.h"
#include "PriestUIManager.generated.h"

class UPriestHUDWidget;
class UPriestDamageNumberWidget;

// 로컬 플레이어마다 엔진이 생성하는 UI 관리자. 맵에 직접 배치할 필요가 없다.
UCLASS()
class PROJECTPRIEST_API UPriestUIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
    void NotifyHitConfirmed(float AppliedDamage, const FVector& DamageLocation);
	// WBP 클래스를 설정한다. 클래스가 달라지면 기존 HUD를 제거한다.
	UFUNCTION(BlueprintCallable, Category="Priest|UI")
	void SetHUDWidgetClass(TSubclassOf<UPriestHUDWidget> WidgetClass);
	// 최초 호출 시 생성하고 이후에는 재사용한다. 생성 또는 화면 추가 실패 시 false를 반환한다.
	UFUNCTION(BlueprintCallable, Category="Priest|UI")
	bool ShowHUD(const FPriestHUDData& Data);
	// 이미 생성된 HUD만 갱신한다. 최초 표시는 ShowHUD를 호출해야 한다.
	UFUNCTION(BlueprintCallable, Category="Priest|UI")
	void UpdateHUD(const FPriestHUDData& Data);
    void UpdateCombatHUD(float Health, float Maximum, const FText& Name, int32 Ammo, int32 Reserve);
	UFUNCTION(BlueprintCallable, Category="Priest|UI")
	void HideHUD(); // 화면에서만 제거한다. 객체는 다시 표시할 때 재사용한다.
	virtual void Deinitialize() override;

private:
    UPROPERTY(Transient) TArray<TObjectPtr<UPriestDamageNumberWidget>> DamageNumbers;
	// UPROPERTY로 참조를 유지해 가비지 컬렉션을 방지한다. Transient는 저장 대상에서 제외한다.
	UPROPERTY(Transient) TObjectPtr<UPriestHUDWidget> HUD;
	UPROPERTY(Transient) TSubclassOf<UPriestHUDWidget> HUDWidgetClass;
};

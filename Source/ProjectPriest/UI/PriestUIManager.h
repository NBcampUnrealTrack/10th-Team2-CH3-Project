#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "PriestHUDData.h"
#include "PriestQuickSlotModel.h"
#include "PriestQuickSlotController.h"
#include "PriestUIManager.generated.h"

class UPriestHUDWidget;
class AIngameGameState;
class UPriestMissionController;
class UPriestDeathWidget;
class UPriestStageClearWidget;
class UPriestStageClearController;
class UPriestDeathController;
class UPriestCombatModel;
class UMvcCharacterStatController;
class APawn;
class APlayerController;
class AIngamePlayerController;
class UPriestDamageNumberWidget;
class UPriestQuickSlotWidget;

USTRUCT()
struct FPriestQuickSlotBinding
{
    GENERATED_BODY()
    UPROPERTY(Transient) TWeakObjectPtr<UPriestQuickSlotWidget> View;
    UPROPERTY(Transient) TObjectPtr<UPriestQuickSlotModel> Model;
    UPROPERTY(Transient) TObjectPtr<UPriestQuickSlotController> Controller;
};

// 로컬 플레이어마다 엔진이 생성하는 UI 관리자. 맵에 직접 배치할 필요가 없다.
UCLASS()
class PROJECTPRIEST_API UPriestUIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
    void ConnectQuickSlot(UPriestQuickSlotWidget* View, int32 SlotIndex);
    void DisconnectQuickSlot(UPriestQuickSlotWidget* View);
    void SetMissionState(AIngameGameState* State, AIngamePlayerController* Owner);
    void SetStageClearWidgetClass(TSubclassOf<UPriestStageClearWidget> WidgetClass);
    UPriestStageClearWidget* ShowStageClearScreen(APlayerController* Owner);
    void HideStageClearScreen();
    void SetDeathWidgetClass(TSubclassOf<UPriestDeathWidget> WidgetClass);
    UPriestDeathWidget* ShowDeathScreen(APlayerController* Owner);
    void HideDeathScreen();
    bool IsHUDDisplayed() const;
    void RestoreHUD();
    void SetCombatPawn(APawn* Pawn, AIngamePlayerController* Owner);
    void DisconnectCombatHUD();
    void NotifyEnemyKilled();
    void NotifyPlayerDamaged();
    void ResetDamageFeedback();
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
	UFUNCTION(BlueprintCallable, Category="Priest|UI")
	void HideHUD(); // 화면에서만 제거한다. 객체는 다시 표시할 때 재사용한다.
	virtual void Deinitialize() override;

private:
    void ConnectCombatView();
    UPROPERTY(Transient) TArray<FPriestQuickSlotBinding> QuickSlotBindings;
    bool bMissionBindingInitialized = false;
    bool bCombatBindingInitialized = false;
    UPROPERTY(Transient) TObjectPtr<UPriestStageClearController> StageClearController;
    UPROPERTY(Transient) TObjectPtr<UPriestStageClearWidget> StageClearWidget;
    UPROPERTY(Transient) TSubclassOf<UPriestStageClearWidget> StageClearWidgetClass;
    UPROPERTY(Transient) TObjectPtr<UPriestMissionController> MissionController;
    UPROPERTY(Transient) TObjectPtr<UPriestDeathController> DeathController;
    UPROPERTY(Transient) TObjectPtr<UPriestDeathWidget> DeathWidget;
    UPROPERTY(Transient) TSubclassOf<UPriestDeathWidget> DeathWidgetClass;
    UPROPERTY(Transient) TObjectPtr<UPriestCombatModel> CombatModel;
    UPROPERTY(Transient) TObjectPtr<UMvcCharacterStatController> CombatController;
    UPROPERTY(Transient) TArray<TObjectPtr<UPriestDamageNumberWidget>> DamageNumbers;
	// UPROPERTY로 참조를 유지해 가비지 컬렉션을 방지한다. Transient는 저장 대상에서 제외한다.
	UPROPERTY(Transient) TObjectPtr<UPriestHUDWidget> HUD;
	UPROPERTY(Transient) TSubclassOf<UPriestHUDWidget> HUDWidgetClass;
};

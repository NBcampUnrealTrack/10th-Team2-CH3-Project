#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcEvents.h"
#include "MvcView.h"
#include "PriestHUDData.h"
#include "PriestHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UMvcControl;
class UPriestDamageNumberWidget;

// WBP에서 배치하고 C++에서 전달받은 데이터를 표시하는 HUD 부모 클래스.
UCLASS(Abstract)
class PROJECTPRIEST_API UPriestHUDWidget 
    : public UUserWidget
    , public IMvcView
{
	GENERATED_BODY()
public:
    void ShowDamageFeedback();
    void ResetDamageFeedback();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Priest|Damage Feedback")
    bool bEnableDamageFeedback = true;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Priest|Damage Feedback")
    FLinearColor DamageFeedbackColor = FLinearColor(0.8f, 0.0f, 0.0f, 0.5f);
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Priest|Damage Feedback", meta=(ClampMin="0.05"))
    float DamageFeedbackDuration = 0.4f;
    // 화면 짧은 변에 대한 가장자리 효과의 너비 비율.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Priest|Damage Feedback", meta=(ClampMin="0.01", ClampMax="0.45"))
    float DamageFeedbackWidth = 0.12f;
    // 비워두면 기본 노란 숫자. WBP 자식에는 DamageText(TextBlock)를 배치한다.
    UPROPERTY(EditDefaultsOnly, Category="Priest|UI")
    TSubclassOf<UPriestDamageNumberWidget> DamageNumberWidgetClass;
    // WBP에서 HitConfirm 애니메이션을 재생한다. 기존 위젯 바인딩은 변경하지 않는다.
    UFUNCTION(BlueprintImplementableEvent, Category="Priest|UI", meta=(DisplayName="On Hit Confirmed"))
    void OnHitConfirmed();
	// 데이터를 보관한 후 표시를 갱신한다. 위젯 생성 전 호출되어도 데이터는 유지된다.
	UFUNCTION(BlueprintCallable, Category="Priest|UI")
	void SetHUDData(const FPriestHUDData& InData);
    UFUNCTION(BlueprintCallable, Category="Priest|UI")
    void SetHealth(int CurrentHealth, int MaxHealth);
    void SetCombatData(float Health, float Maximum, const FText& Name, int32 Ammo, int32 Reserve);

    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle DelegateHandle) override;
    virtual void InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter) override;

protected:
	// WBP의 위젯 바인딩 이후 보관한 데이터를 반영한다.
	virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& Geometry,
        const FSlateRect& CullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& Style, bool bParentEnabled) const override;

private:
    float DamageFeedbackRemaining = 0.0f;
	// 보관한 데이터를 화면에 반영한다. 게임 상태 자체는 변경하지 않는다.
	void Refresh();
	UPROPERTY(Transient) FPriestHUDData Data;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> HealthText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> WeaponText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> MissionText;


protected:
    FViewEventRaisedDelegate Listener;
};

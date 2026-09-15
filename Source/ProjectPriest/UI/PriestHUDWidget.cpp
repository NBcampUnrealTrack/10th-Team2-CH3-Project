#include "PriestHUDWidget.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MvcControl.h"

bool UPriestHUDWidget::Initialize()
{
    if (bBindingsReady)
    {
        return true;
    }
    JASSERT_BOOL((Super::Initialize()), "%hs: Base widget initialization failed", __FUNCTION__);
    JASSERT_BOOL((IsValid(HealthBar)), "%hs: Missing required HealthBar binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(HealthText)), "%hs: Missing required HealthText binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(WeaponText)), "%hs: Missing required WeaponText binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(MissionText)), "%hs: Missing required MissionText binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(DamageFeedback)), "%hs: Missing required DamageFeedback binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(KillNotification)), "%hs: Missing required KillNotification binding", __FUNCTION__);
    bBindingsReady = true;
    return true;
}

void UPriestHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
    if (!bBindingsReady)
    {
        return;
    }
    ResetDamageFeedback();
    ResetKillNotification();
	Refresh();
}

void UPriestHUDWidget::SetHUDData(const FPriestHUDData& InData)
{
	Data = InData;
	Refresh();
}

void UPriestHUDWidget::SetHealth(int CurrentHealth, int MaxHealth)
{
    Data.Health = CurrentHealth;
    Data.MaxHealth = MaxHealth;
    Refresh();
}

void UPriestHUDWidget::SetCombatData(float Health, float Maximum, const FText& Name, int32 Ammo, int32 Reserve)
{
    Data.Health = Health;
    Data.MaxHealth = Maximum;
    Data.WeaponName = Name;
    Data.MagazineAmmo = Ammo;
    Data.ReserveAmmo = Reserve;
    Refresh();
}

void UPriestHUDWidget::Refresh()
{
    // Preserve data supplied before widget initialization.
    if (!bBindingsReady)
    {
        return;
    }
	// 비정상 수치와 체력 범위를 표시 단계에서 보정하고, 최대 체력 0의 나눗셈을 방지한다.
	const float Maximum = FMath::IsFinite(Data.MaxHealth) ? FMath::Max(0.0f, Data.MaxHealth) : 0.0f;
	const float Health = FMath::IsFinite(Data.Health) ? FMath::Clamp(Data.Health, 0.0f, Maximum) : 0.0f;
	HealthBar->SetPercent(Maximum > 0.0f ? Health / Maximum : 0.0f);
	HealthText->SetText(FText::Format(NSLOCTEXT("PriestHUD", "Health", "HP {0} / {1}"), FText::AsNumber(FMath::FloorToInt(Health)), FText::AsNumber(FMath::FloorToInt(Maximum))));
	WeaponText->SetText(FText::Format(NSLOCTEXT("PriestHUD", "Ammo", "{0}\n{1} / {2}"), Data.WeaponName, FText::AsNumber(FMath::Max(0, Data.MagazineAmmo)), FText::AsNumber(FMath::Max(0, Data.ReserveAmmo))));
	MissionText->SetText(Data.MissionObjective);
	// 경과 시간은 Data.ElapsedSeconds에 보관한다.
}

FDelegateHandle UPriestHUDWidget::AddListener(UMvcControl* Control)
{
    return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent);
}

void UPriestHUDWidget::RemoveListener(FDelegateHandle DelegateHandle)
{
    Listener.Remove(DelegateHandle);
}

void UPriestHUDWidget::InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter)
{
    Listener.Broadcast(this, EventName, Parameter);
}
void UPriestHUDWidget::ShowDamageFeedback()
{
    if (!bEnableDamageFeedback)
    {
        return;
    }
    DamageFeedbackRemaining = FMath::Max(0.05f, DamageFeedbackDuration);
    UpdateFeedbackOpacity();
}

void UPriestHUDWidget::ResetDamageFeedback()
{
    DamageFeedbackRemaining = 0.0f;
    UpdateFeedbackOpacity();
}

void UPriestHUDWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);
    if (KillNotificationRemaining > 0.0f)
    {
        KillNotificationRemaining = FMath::Max(0.0f, KillNotificationRemaining - DeltaTime);
        UpdateFeedbackOpacity();
    }
    if (DamageFeedbackRemaining > 0.0f)
    {
        DamageFeedbackRemaining = FMath::Max(0.0f, DamageFeedbackRemaining - DeltaTime);
        UpdateFeedbackOpacity();
    }
}

void UPriestHUDWidget::UpdateFeedbackOpacity()
{
    if (!bBindingsReady)
    {
        return;
    }
    const float Opacity = bEnableDamageFeedback
        ? FMath::Clamp(DamageFeedbackRemaining / FMath::Max(0.05f, DamageFeedbackDuration), 0.0f, 1.0f)
        : 0.0f;
    DamageFeedback->SetRenderOpacity(Opacity);
    const float FadeDuration = FMath::Min(0.3f, FMath::Max(0.1f, KillNotificationDuration));
    KillNotification->SetRenderOpacity(FMath::Clamp(KillNotificationRemaining / FadeDuration, 0.0f, 1.0f));
}

void UPriestHUDWidget::ShowKillNotification()
{
    KillNotificationRemaining = FMath::Max(0.1f, KillNotificationDuration);
    UpdateFeedbackOpacity();
}

void UPriestHUDWidget::ResetKillNotification()
{
    KillNotificationRemaining = 0.0f;
    UpdateFeedbackOpacity();
}

void UPriestHUDWidget::SetMissionObjective(const FText& Objective)
{
    Data.MissionObjective = Objective;
    if (bBindingsReady)
    {
        MissionText->SetText(Data.MissionObjective);
    }
}

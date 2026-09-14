#include "PriestHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MvcControl.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"

void UPriestHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
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
	// 데이터가 화면 생성보다 먼저 들어오면 보관만 하고, 생성 후 다시 반영한다.
	if (!HealthBar || !HealthText || !WeaponText || !MissionText) return;
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
    if (!bEnableDamageFeedback) return;
    DamageFeedbackRemaining = FMath::Max(0.05f, DamageFeedbackDuration);
    InvalidateLayoutAndVolatility();
}

void UPriestHUDWidget::ResetDamageFeedback()
{
    DamageFeedbackRemaining = 0.0f;
    InvalidateLayoutAndVolatility();
}

void UPriestHUDWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);
    if (KillNotificationRemaining > 0.0f)
    {
        KillNotificationRemaining = FMath::Max(0.0f, KillNotificationRemaining - DeltaTime);
        InvalidateLayoutAndVolatility();
    }
    if (DamageFeedbackRemaining > 0.0f)
    {
        DamageFeedbackRemaining = FMath::Max(0.0f, DamageFeedbackRemaining - DeltaTime);
        InvalidateLayoutAndVolatility();
    }
}

int32 UPriestHUDWidget::NativePaint(const FPaintArgs& Args, const FGeometry& Geometry,
    const FSlateRect& CullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& Style, bool bParentEnabled) const
{
    int32 TopLayer = Super::NativePaint(Args, Geometry, CullingRect, OutDrawElements, LayerId, Style, bParentEnabled);
    if (KillNotificationRemaining > 0.0f && !KillNotificationText.IsEmpty())
    {
        const FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Bold", FMath::Clamp(KillNotificationFontSize, 8, 120));
        const FString Text = KillNotificationText.ToString();
        const FVector2D TextSize = FSlateApplication::Get().GetRenderer()->GetFontMeasureService()->Measure(Text, Font);
        const FVector2D ScreenSize = Geometry.GetLocalSize();
        const FVector2D Position = FVector2D(ScreenSize.X * FMath::Clamp(KillNotificationPosition.X, 0.0, 1.0),
            ScreenSize.Y * FMath::Clamp(KillNotificationPosition.Y, 0.0, 1.0)) - TextSize * 0.5;
        FLinearColor Color = KillNotificationColor;
        // 마지막 0.3초에만 페이드아웃한다.
        Color.A *= FMath::Clamp(KillNotificationRemaining / FMath::Min(0.3f, FMath::Max(0.1f, KillNotificationDuration)), 0.0f, 1.0f)
            * Style.GetColorAndOpacityTint().A;
        FSlateDrawElement::MakeText(OutDrawElements, ++TopLayer,
            Geometry.ToPaintGeometry(FVector2f(TextSize), FSlateLayoutTransform(FVector2f(Position + FVector2D(1, 1)))),
            Text, Font, ESlateDrawEffect::None, FLinearColor(0, 0, 0, Color.A));
        FSlateDrawElement::MakeText(OutDrawElements, ++TopLayer,
            Geometry.ToPaintGeometry(FVector2f(TextSize), FSlateLayoutTransform(FVector2f(Position))),
            Text, Font, ESlateDrawEffect::None, Color);
    }
    if (!bEnableDamageFeedback || DamageFeedbackRemaining <= 0.0f) return TopLayer;
    const FVector2D Size = Geometry.GetLocalSize();
    const float Width = FMath::Min(Size.X, Size.Y) * FMath::Clamp(DamageFeedbackWidth, 0.01f, 0.45f);
    if (Width <= 0.0f) return TopLayer;
    const float Fade = FMath::Clamp(DamageFeedbackRemaining / FMath::Max(0.05f, DamageFeedbackDuration), 0.0f, 1.0f);
    const FSlateBrush* Brush = FCoreStyle::Get().GetBrush("WhiteBrush");
    const int32 EffectLayer = TopLayer + 1;
    // 서로 겹치지 않는 사각 고리로 중앙이 투명한 가장자리 그라데이션을 만든다.
    constexpr int32 Steps = 32;
    for (int32 Index = 0; Index < Steps; ++Index)
    {
        const float Inset = Width * Index / Steps;
        const float Thickness = Width / Steps;
        FLinearColor Color = DamageFeedbackColor;
        const float Edge = 1.0f - (Index + 0.5f) / Steps;
        Color.A *= Edge * Edge * Fade * Style.GetColorAndOpacityTint().A;
        auto Draw = [&](FVector2D Position, FVector2D Extent)
        {
            FSlateDrawElement::MakeBox(OutDrawElements, EffectLayer,
                Geometry.ToPaintGeometry(FVector2f(Extent), FSlateLayoutTransform(FVector2f(Position))),
                Brush, ESlateDrawEffect::None, Color);
        };
        Draw({Inset, Inset}, {Size.X - 2 * Inset, Thickness});
        Draw({Inset, Size.Y - Inset - Thickness}, {Size.X - 2 * Inset, Thickness});
        Draw({Inset, Inset + Thickness}, {Thickness, Size.Y - 2 * (Inset + Thickness)});
        Draw({Size.X - Inset - Thickness, Inset + Thickness}, {Thickness, Size.Y - 2 * (Inset + Thickness)});
    }
    return EffectLayer;
}

void UPriestHUDWidget::ShowKillNotification()
{
    KillNotificationRemaining = FMath::Max(0.1f, KillNotificationDuration);
    InvalidateLayoutAndVolatility();
}

void UPriestHUDWidget::ResetKillNotification()
{
    KillNotificationRemaining = 0.0f;
    InvalidateLayoutAndVolatility();
}

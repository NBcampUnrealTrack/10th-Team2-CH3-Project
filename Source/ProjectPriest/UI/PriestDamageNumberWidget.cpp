#include "PriestDamageNumberWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"

void UPriestDamageNumberWidget::InitializeDamage(float Amount, const FVector& Location)
{
    Damage = Amount;
    WorldLocation = Location;
    Age = 0.0f;
    RefreshText();
}

void UPriestDamageNumberWidget::RefreshText()
{
    if (!DamageText)
    {
        return;
    }
    FNumberFormattingOptions Format;
    Format.SetMaximumFractionalDigits(1);
    Format.SetMinimumFractionalDigits(0);
    DamageText->SetText(FText::AsNumber(Damage, &Format));
}

void UPriestDamageNumberWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshText();
    SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
    // 첫 위치 계산 전 화면 모서리에서 깜박이지 않도록 한다.
    SetRenderOpacity(0.0f);
}

void UPriestDamageNumberWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);
    Age += DeltaTime;
    const float Duration = FMath::Max(0.1f, Lifetime);
    if (Age >= Duration || !GetOwningPlayer())
    {
        RemoveFromParent();
        return;
    }
    FVector2D Position;
    if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), WorldLocation, Position, true))
    {
        SetRenderOpacity(0.0f);
        return;
    }
    const float Progress = Age / Duration;
    Position.Y -= RiseDistance * Progress;
    // ProjectWorldLocationToWidgetPosition은 이미 DPI를 보정한다.
    SetPositionInViewport(Position, false);
    SetRenderOpacity(1.0f - Progress);
}

#include "PriestDamageNumberWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"

TSharedRef<SWidget> UPriestDamageNumberWidget::RebuildWidget()
{
    if (!WidgetTree->RootWidget)
    {
        DamageText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("DamageText"));
        WidgetTree->RootWidget = DamageText;
        FSlateFontInfo Font = DamageText->GetFont();
        Font.Size = 28;
        DamageText->SetFont(Font);
        DamageText->SetColorAndOpacity(FLinearColor(1.0f, 0.85f, 0.25f));
        DamageText->SetShadowColorAndOpacity(FLinearColor::Black);
        DamageText->SetShadowOffset(FVector2D(1.0f, 1.0f));
        DamageText->SetJustification(ETextJustify::Center);
    }
    return Super::RebuildWidget();
}

void UPriestDamageNumberWidget::InitializeDamage(float Amount, const FVector& Location)
{
    Damage = Amount;
    WorldLocation = Location;
    Age = 0.0f;
    RefreshText();
}

void UPriestDamageNumberWidget::RefreshText()
{
    if (!DamageText) return;
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

#include "PriestHUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MvcControl.h"
#include "JUtility.h"

TSharedRef<SWidget> UPriestHUDWidget::RebuildWidget()
{
	if (!WidgetTree->RootWidget)
	{
		// 최초 한 번만 트리를 만들어 재구성 시 자식 위젯의 중복 생성을 막는다.
		UCanvasPanel* Canvas = WidgetTree->ConstructWidget<UCanvasPanel>();
		WidgetTree->RootWidget = Canvas;
		// Anchor는 화면 기준점(0~1), Alignment는 위젯 내부 기준점(0~1)이다.
		// Position은 기준점에서의 거리, Size는 크기이며 둘 다 DPI 배율 적용 전 UI 단위다.
		auto Place = [Canvas](UWidget* Widget, FVector2D Anchor, FVector2D Position, FVector2D Size, FVector2D Alignment)
		{
			UCanvasPanelSlot* Slot = Canvas->AddChildToCanvas(Widget);
			Slot->SetAnchors(FAnchors(Anchor.X, Anchor.Y));
			Slot->SetAlignment(Alignment);
			Slot->SetPosition(Position);
			Slot->SetSize(Size);
		};
		// 모든 텍스트에 흰색 글자와 검정 그림자를 공통 적용한다.
		auto Text = [this](int32 Size)
		{
			UTextBlock* Result = WidgetTree->ConstructWidget<UTextBlock>();
			FSlateFontInfo Font = Result->GetFont();
			Font.Size = Size;
			Result->SetFont(Font);
			Result->SetColorAndOpacity(FSlateColor(FLinearColor::White));
			Result->SetShadowOffset(FVector2D(1, 1));
			Result->SetShadowColorAndOpacity(FLinearColor::Black);
			return Result;
		};
		// 좌측 하단: 체력 수치와 체력바. 음수 Y는 화면 아래쪽에서 위로 올리는 거리다.
		HealthText = Text(20);
		Place(HealthText, {0, 1}, {40, -100}, {300, 32}, {0, 0});
		HealthBar = WidgetTree->ConstructWidget<UProgressBar>();
		HealthBar->SetFillColorAndOpacity(FLinearColor(0.15f, 0.8f, 0.3f));
		Place(HealthBar, {0, 1}, {40, -60}, {280, 20}, {0, 0});
		// 우측 하단: 무기 이름과 탄약을 두 줄로 표시한다.
		WeaponText = Text(22);
		WeaponText->SetJustification(ETextJustify::Right);
		Place(WeaponText, {1, 1}, {-40, -110}, {360, 80}, {1, 0});
		// 우측 중앙: 반투명 박스 안에 미션 표시. 긴 문구는 자동 줄바꿈한다.
		MissionText = Text(20);
		MissionText->SetAutoWrapText(true);
		UBorder* MissionPanel = WidgetTree->ConstructWidget<UBorder>();
		MissionPanel->SetBrushColor(FLinearColor(0.02f, 0.02f, 0.02f, 0.6f)); // 마지막 값은 불투명도.
		MissionPanel->SetPadding(FMargin(20.0f));
		MissionPanel->SetVerticalAlignment(VAlign_Center);
		MissionPanel->SetContent(MissionText);
		// 박스 크기 360×160, 화면 오른쪽에서 40만큼 띄우고 세로 중앙에 고정한다.
		Place(MissionPanel, {1, 0.5f}, {-40, 0}, {360, 160}, {1, 0.5f});
		// 화면 정중앙에 임시 조준점을 배치한다.
		UTextBlock* Crosshair = Text(28);
		Crosshair->SetText(FText::FromString(TEXT("+")));
		Crosshair->SetJustification(ETextJustify::Center);
		Place(Crosshair, {0.5f, 0.5f}, {0, 0}, {40, 40}, {0.5f, 0.5f});
	}
	Refresh();
	return Super::RebuildWidget();
}

void UPriestHUDWidget::SetHUDData(const FPriestHUDData& InData)
{
	Data = InData;
	Refresh();
}

void UPriestHUDWidget::SetHealth(int CurrentHealth, int MaxHealth)
{
    JASSERT(MaxHealth == 0, "Can not divde by zero");

    float FCurrentHealth = (float)CurrentHealth;
    float FMaxHealth = (float)MaxHealth;
    float Percent = FCurrentHealth / FMaxHealth;

    HealthBar->SetPercent(Percent);

    HealthText->SetText(FText::Format(NSLOCTEXT("PriestHUD", "Health", "HP {0} / {1}"), CurrentHealth, MaxHealth));
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
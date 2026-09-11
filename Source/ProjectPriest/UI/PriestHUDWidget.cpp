#include "PriestHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MvcControl.h"

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
#include "PriestUIManager.h"
#include "PriestHUDWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

void UPriestUIManager::SetHUDWidgetClass(TSubclassOf<UPriestHUDWidget> WidgetClass)
{
    if (HUDWidgetClass == WidgetClass) return;
    HideHUD();
    HUD = nullptr;
    HUDWidgetClass = WidgetClass;
}

bool UPriestUIManager::ShowHUD(const FPriestHUDData& Data)
{
    if (!HUDWidgetClass || HUDWidgetClass->HasAnyClassFlags(CLASS_Abstract))
    {
        UE_LOG(LogTemp, Warning, TEXT("Priest HUD: Set a concrete Widget Blueprint class before ShowHUD."));
        return false;
    }
	APlayerController* Controller = GetLocalPlayer()->GetPlayerController(GetWorld());
	if (!Controller) return false;
	// 레벨 이동 등으로 소유 컨트롤러가 달라지면 기존 위젯 대신 새 위젯을 만든다.
	if (HUD && HUD->GetOwningPlayer() != Controller)
	{
		HUD->RemoveFromParent();
		HUD = nullptr;
	}
	if (!HUD) HUD = CreateWidget<UPriestHUDWidget>(Controller, HUDWidgetClass);
	if (!HUD) return false;
	HUD->SetHUDData(Data);
	// HUD와 자식 위젯이 마우스 입력을 가로채지 않도록 표시 전용으로 설정한다.
	HUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	// 중복 추가를 막는다. ZOrder 0은 기본 HUD 레이어다.
	if (!HUD->IsInViewport()) return HUD->AddToPlayerScreen(0);
	return true;
}

void UPriestUIManager::UpdateHUD(const FPriestHUDData& Data)
{
	if (HUD) HUD->SetHUDData(Data);
}

void UPriestUIManager::UpdateCombatHUD(float Health, float Maximum, const FText& Name, int32 Ammo, int32 Reserve)
{
    if (HUD) HUD->SetCombatData(Health, Maximum, Name, Ammo, Reserve);
}

void UPriestUIManager::HideHUD()
{
	if (HUD) HUD->RemoveFromParent();
}

void UPriestUIManager::Deinitialize()
{
	// 서브시스템 종료 시 화면과 보유 참조를 정리한다.
	HideHUD();
	HUD = nullptr;
	HUDWidgetClass = nullptr;
	Super::Deinitialize();
}

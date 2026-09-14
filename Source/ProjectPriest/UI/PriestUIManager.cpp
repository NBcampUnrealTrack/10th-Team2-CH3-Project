#include "PriestUIManager.h"
#include "PriestDeathWidget.h"
#include "PriestDeathController.h"
#include "IngamePlayerController.h"
#include "PriestCombatModel.h"
#include "MvcCharacterStatController.h"
#include "PriestHUDWidget.h"
#include "PriestDamageNumberWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

void UPriestUIManager::SetHUDWidgetClass(TSubclassOf<UPriestHUDWidget> WidgetClass)
{
    if (HUDWidgetClass == WidgetClass)
    {
        return;
    }
    HideHUD();
    if (CombatController) CombatController->SetView(nullptr);
    HUD = nullptr;
    HUDWidgetClass = WidgetClass;
}

bool UPriestUIManager::ShowHUD(const FPriestHUDData& Data)
{
    if (DeathController && DeathController->IsDeathActive()) return false;
    if (!HUDWidgetClass || HUDWidgetClass->HasAnyClassFlags(CLASS_Abstract))
    {
        UE_LOG(LogTemp, Warning, TEXT("Priest HUD: Set a concrete Widget Blueprint class before ShowHUD."));
        return false;
    }
	APlayerController* Controller = GetLocalPlayer()->GetPlayerController(GetWorld());
	if (!Controller)
	{
	    return false;
	}
	// 레벨 이동 등으로 소유 컨트롤러가 달라지면 기존 위젯 대신 새 위젯을 만든다.
	if (HUD && HUD->GetOwningPlayer() != Controller)
	{
		HideHUD();
		if (CombatController) CombatController->SetView(nullptr);
		HUD = nullptr;
	}
	if (!HUD)
	{
	    HUD = CreateWidget<UPriestHUDWidget>(Controller, HUDWidgetClass);
	}
	if (!HUD)
	{
	    return false;
	}
	HUD->SetHUDData(Data);
    ConnectCombatView();
	// HUD와 자식 위젯이 마우스 입력을 가로채지 않도록 표시 전용으로 설정한다.
	HUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	// 중복 추가를 막는다. ZOrder 0은 기본 HUD 레이어다.
	if (!HUD->IsInViewport())
	{
	    return HUD->AddToPlayerScreen(0);
	}
	return true;
}

void UPriestUIManager::UpdateHUD(const FPriestHUDData& Data)
{
	if (HUD)
	{
	    HUD->SetHUDData(Data);
	}
}

void UPriestUIManager::NotifyHitConfirmed(float AppliedDamage, const FVector& DamageLocation)
{
    // 숨겨진 HUD나 이전 레벨의 HUD에는 일회성 효과를 전달하지 않는다.
    APlayerController* Controller = GetLocalPlayer()->GetPlayerController(GetWorld());
    if (IsValid(HUD) && HUD->GetOwningPlayer() == Controller && HUD->IsInViewport() && HUD->IsVisible())
    {
        HUD->OnHitConfirmed();
        if (!FMath::IsFinite(AppliedDamage) || AppliedDamage <= 0.0f || DamageLocation.ContainsNaN())
        {
            return;
        }
        DamageNumbers.RemoveAll([](const TObjectPtr<UPriestDamageNumberWidget>& Number)
        {
            return !IsValid(Number) || !Number->IsInViewport();
        });
        // 범위 공격/연속 명중으로 일시 위젯이 무한히 늘어나지 않도록 제한한다.
        if (DamageNumbers.Num() >= 32)
        {
            DamageNumbers[0]->RemoveFromParent();
            DamageNumbers.RemoveAt(0);
        }
        TSubclassOf<UPriestDamageNumberWidget> NumberClass = HUD->DamageNumberWidgetClass;
        if (!NumberClass || NumberClass->HasAnyClassFlags(CLASS_Abstract))
        {
            UE_LOG(LogTemp, Warning, TEXT("Priest HUD: Set DamageNumberWidgetClass to a concrete Widget Blueprint."));
            return;
        }
        UPriestDamageNumberWidget* Number = CreateWidget<UPriestDamageNumberWidget>(Controller, NumberClass);
        if (Number)
        {
            Number->InitializeDamage(AppliedDamage, DamageLocation);
            Number->SetVisibility(ESlateVisibility::HitTestInvisible);
            if (Number->AddToPlayerScreen(10))
            {
                DamageNumbers.Add(Number);
            }
        }
    }
}

void UPriestUIManager::HideHUD()
{
    ResetDamageFeedback();
    for (UPriestDamageNumberWidget* Number : DamageNumbers)
    {
        if (IsValid(Number))
        {
            Number->RemoveFromParent();
        }
    }
    DamageNumbers.Reset();
	if (HUD)
	{
	    HUD->RemoveFromParent();
	}
}

void UPriestUIManager::Deinitialize()
{
	// 서브시스템 종료 시 화면과 보유 참조를 정리한다.
    DisconnectCombatHUD();
	HUD = nullptr;
	HUDWidgetClass = nullptr;
	Super::Deinitialize();
}

void UPriestUIManager::NotifyPlayerDamaged()
{
    APlayerController* Controller = GetLocalPlayer()->GetPlayerController(GetWorld());
    if (IsValid(HUD) && HUD->GetOwningPlayer() == Controller && HUD->IsInViewport() && HUD->IsVisible())
    {
        HUD->ShowDamageFeedback();
    }
}

void UPriestUIManager::ResetDamageFeedback()
{
    if (IsValid(HUD))
    {
        HUD->ResetDamageFeedback();
        HUD->ResetKillNotification();
    }
}

void UPriestUIManager::NotifyEnemyKilled()
{
    APlayerController* Controller = GetLocalPlayer()->GetPlayerController(GetWorld());
    if (IsValid(HUD) && HUD->GetOwningPlayer() == Controller && HUD->IsInViewport() && HUD->IsVisible())
    {
        HUD->ShowKillNotification();
    }
}

void UPriestUIManager::SetCombatPawn(APawn* Pawn, AIngamePlayerController* Owner)
{
    if (!CombatModel) CombatModel = NewObject<UPriestCombatModel>(this);
    if (!CombatController) CombatController = NewObject<UMvcCharacterStatController>(this);
    CombatController->SetModel(CombatModel);
    CombatController->SetView(HUD);
    ResetDamageFeedback();
    if (!DeathController)
    {
        DeathController = NewObject<UPriestDeathController>(this);
        DeathController->Initialize(CombatModel, this, Owner);
    }
    CombatModel->SetPawn(Pawn);
}

void UPriestUIManager::ConnectCombatView()
{
    if (CombatController && CombatModel)
    {
        CombatController->SetView(HUD);
        CombatController->HandleModelChanged(CombatModel, 0);
    }
}

void UPriestUIManager::DisconnectCombatHUD()
{
    if (DeathController) DeathController->Disconnect();
    HideDeathScreen();
    DeathController = nullptr;
    if (CombatController) CombatController->Disconnect();
    if (CombatModel) CombatModel->Disconnect();
    CombatController = nullptr;
    CombatModel = nullptr;
    HideHUD();
}

void UPriestUIManager::SetDeathWidgetClass(TSubclassOf<UPriestDeathWidget> WidgetClass)
{
    DeathWidgetClass = WidgetClass;
}

UPriestDeathWidget* UPriestUIManager::ShowDeathScreen(APlayerController* Owner)
{
    if (!IsValid(Owner)) return nullptr;
    if (DeathWidget && DeathWidget->GetOwningPlayer() != Owner) HideDeathScreen();
    if (!DeathWidget && DeathWidgetClass && !DeathWidgetClass->HasAnyClassFlags(CLASS_Abstract))
    {
        DeathWidget = CreateWidget<UPriestDeathWidget>(Owner, DeathWidgetClass);
    }
    if (DeathWidget && (DeathWidget->IsInViewport() || DeathWidget->AddToPlayerScreen(100)))
    {
        DeathWidget->SetVisibility(ESlateVisibility::Visible);
        return DeathWidget;
    }
    HideDeathScreen();
    UE_LOG(LogTemp, Error, TEXT("Priest Death UI: Set DeathWidgetClass to a configured death Widget Blueprint in the ingame PlayerController."));
    return nullptr;
}

void UPriestUIManager::HideDeathScreen()
{
    if (DeathWidget) DeathWidget->RemoveFromParent();
    DeathWidget = nullptr;
}

bool UPriestUIManager::IsHUDDisplayed() const
{
    return IsValid(HUD) && HUD->IsInViewport();
}

void UPriestUIManager::RestoreHUD()
{
    APlayerController* Owner = GetLocalPlayer()->GetPlayerController(GetWorld());
    if (HUD && HUD->GetOwningPlayer() == Owner && !HUD->IsInViewport())
    {
        ConnectCombatView();
        HUD->AddToPlayerScreen(0);
    }
}

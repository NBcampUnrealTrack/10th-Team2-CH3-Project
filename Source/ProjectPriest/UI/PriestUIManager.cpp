#include "PriestUIManager.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "PriestMissionController.h"
#include "IngameGameState.h"
#include "PriestDeathWidget.h"
#include "PriestStageClearWidget.h"
#include "PriestStageClearController.h"
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
    if (CombatController)
    {
        CombatController->SetView(nullptr);
        CombatController->SetModel(nullptr);
    }
    if (MissionController)
    {
        MissionController->SetView(nullptr);
    }
    HUD = nullptr;
    HUDWidgetClass = WidgetClass;
}

bool UPriestUIManager::ShowHUD(const FPriestHUDData& Data)
{
    if (DeathController && DeathController->IsDeathActive())
    {
        return false;
    }
    if (StageClearController && StageClearController->IsStageClearActive())
    {
        return false;
    }
    if (!HUDWidgetClass || HUDWidgetClass->HasAnyClassFlags(CLASS_Abstract))
    {
        JWarning("Priest HUD: Set a concrete Widget Blueprint class before ShowHUD.");
        return false;
    }
	APlayerController* Controller = GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
	JASSERT_BOOL((IsValid(Controller)), "%hs [%s]: No PlayerController is available to create the HUD.", __FUNCTION__, *GetNameSafe(this));
	// 레벨 이동 등으로 소유 컨트롤러가 달라지면 기존 위젯 대신 새 위젯을 만든다.
	if (HUD && HUD->GetOwningPlayer() != Controller)
	{
		HideHUD();
		if (CombatController)
		{
			CombatController->SetView(nullptr);
			CombatController->SetModel(nullptr);
		}
		if (MissionController)
		{
			MissionController->SetView(nullptr);
		}
		HUD = nullptr;
	}
	if (!HUD)
	{
	    HUD = CreateWidget<UPriestHUDWidget>(Controller, HUDWidgetClass);
	}
	JASSERT_BOOL((IsValid(HUD)), "%hs [%s]: CreateWidget failed for HUDWidgetClass.", __FUNCTION__, *GetNameSafe(this));
    if (!HUD->HasValidBindings())
    {
        JError("HUD initialization failed. Check its required widget bindings");
        HUD = nullptr;
        return false;
    }
	HUD->SetHUDData(Data);
    ConnectCombatView();
	// HUD와 자식 위젯이 마우스 입력을 가로채지 않도록 표시 전용으로 설정한다.
	HUD->SetVisibility(ESlateVisibility::HitTestInvisible);
	// 중복 추가를 막는다. ZOrder 0은 기본 HUD 레이어다.
	if (!HUD->IsInViewport())
	{
        const bool bDisplayed = HUD->AddToPlayerScreen(0);
        if (!bDisplayed)
        {
            JError("%hs [%s]: Failed to add HUD to the player screen.", __FUNCTION__, *GetNameSafe(this));
        }
        return bDisplayed;
	}
	return true;
}

void UPriestUIManager::UpdateHUD(const FPriestHUDData& Data)
{
    JASSERT((IsValid(HUD)), "%hs [%s]: HUD is missing. Call ShowHUD before UpdateHUD.", __FUNCTION__, *GetNameSafe(this));
    HUD->SetHUDData(Data);
}

void UPriestUIManager::NotifyHitConfirmed(float AppliedDamage, const FVector& DamageLocation)
{
    // 숨겨진 HUD나 이전 레벨의 HUD에는 일회성 효과를 전달하지 않는다.
    APlayerController* Controller = GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
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
            JWarning("Priest HUD: Set DamageNumberWidgetClass to a concrete Widget Blueprint.");
            return;
        }
        UPriestDamageNumberWidget* Number = CreateWidget<UPriestDamageNumberWidget>(Controller, NumberClass);
        if (Number && Number->HasValidBindings())
        {
            Number->InitializeDamage(AppliedDamage, DamageLocation);
            Number->SetVisibility(ESlateVisibility::HitTestInvisible);
            if (Number->AddToPlayerScreen(10))
            {
                DamageNumbers.Add(Number);
            }
            else
            {
                JError("%hs [%s]: Failed to display damage number widget.", __FUNCTION__, *GetNameSafe(this));
            }
        }
        else
        {
            JError("%hs [%s]: Failed to create damage number widget.", __FUNCTION__, *GetNameSafe(this));
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
    APlayerController* Controller = GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
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
    APlayerController* Controller = GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
    if (IsValid(HUD) && HUD->GetOwningPlayer() == Controller && HUD->IsInViewport() && HUD->IsVisible())
    {
        HUD->ShowKillNotification();
    }
}

void UPriestUIManager::SetCombatPawn(APawn* Pawn, AIngamePlayerController* Owner)
{
    JASSERT((IsValid(Owner)), "%hs [%s]: Combat binding requires a valid IngamePlayerController.", __FUNCTION__, *GetNameSafe(this));
    bCombatBindingInitialized = true;
    if (!CombatModel)
    {
        CombatModel = NewObject<UPriestCombatModel>(this);
    }
    if (!CombatController)
    {
        CombatController = NewObject<UMvcCharacterStatController>(this);
    }
    CombatController->SetModel(IsValid(HUD) ? CombatModel.Get() : nullptr);
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
    JASSERT((IsValid(HUD)), "%hs [%s]: HUD is missing. Create the HUD before connecting its controllers.", __FUNCTION__, *GetNameSafe(this));
    if (IsValid(MissionController))
    {
        MissionController->SetView(HUD);
        MissionController->HandleModelChanged(MissionController->GetModel<AIngameGameState>(), 0);
    }
    else if (bMissionBindingInitialized)
    {
        JError("%hs [%s]: MissionController is missing after SetMissionState. Reinitialize mission binding.", __FUNCTION__, *GetNameSafe(this));
    }
    if (IsValid(CombatController) && IsValid(CombatModel))
    {
        CombatController->SetView(HUD);
        CombatController->SetModel(CombatModel);
        CombatController->HandleModelChanged(CombatModel, 0);
    }
    else if (bCombatBindingInitialized)
    {
        if (!IsValid(CombatController))
        {
            JError("%hs [%s]: CombatController is invalid after SetCombatPawn.", __FUNCTION__, *GetNameSafe(this));
        }
        if (!IsValid(CombatModel))
        {
            JError("%hs [%s]: CombatModel is invalid after SetCombatPawn.", __FUNCTION__, *GetNameSafe(this));
        }
    }
}

void UPriestUIManager::DisconnectCombatHUD()
{
    bCombatBindingInitialized = false;
    bMissionBindingInitialized = false;
    if (StageClearController)
    {
        StageClearController->Disconnect();
    }
    HideStageClearScreen();
    StageClearController = nullptr;
    if (MissionController)
    {
        MissionController->Disconnect();
    }
    MissionController = nullptr;
    if (DeathController)
    {
        DeathController->Disconnect();
    }
    HideDeathScreen();
    DeathController = nullptr;
    if (CombatController)
    {
        CombatController->Disconnect();
    }
    if (CombatModel)
    {
        CombatModel->Disconnect();
    }
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
    JASSERT_NULLPTR((IsValid(Owner)), "%hs [%s]: A valid owning PlayerController is required to create the Death screen.", __FUNCTION__, *GetNameSafe(this));
    JASSERT_NULLPTR((DeathWidgetClass && !DeathWidgetClass->HasAnyClassFlags(CLASS_Abstract)), "%hs [%s]: Configure a concrete DeathWidgetClass in the ingame PlayerController Blueprint.", __FUNCTION__, *GetNameSafe(this));
    if (IsValid(DeathWidget) && DeathWidget->GetOwningPlayer() != Owner)
    {
        HideDeathScreen();
    }
    if (!IsValid(DeathWidget))
    {
        DeathWidget = CreateWidget<UPriestDeathWidget>(Owner, DeathWidgetClass);
    }
    JASSERT_NULLPTR((IsValid(DeathWidget)), "%hs [%s]: CreateWidget failed for DeathWidgetClass.", __FUNCTION__, *GetNameSafe(this));
    if (!DeathWidget->HasValidBindings())
    {
        JError("Death widget initialization failed");
        HideDeathScreen();
        return nullptr;
    }
    if (!DeathWidget->IsInViewport() && !DeathWidget->AddToPlayerScreen(100))
    {
        JError("%hs [%s]: Failed to add the Death screen to the owning player.", __FUNCTION__, *GetNameSafe(this));
        HideDeathScreen();
        return nullptr;
    }
    DeathWidget->SetVisibility(ESlateVisibility::Visible);
    return DeathWidget;
}

void UPriestUIManager::HideDeathScreen()
{
    if (DeathWidget)
    {
        DeathWidget->RemoveFromParent();
    }
    DeathWidget = nullptr;
}

bool UPriestUIManager::IsHUDDisplayed() const
{
    return IsValid(HUD) && HUD->IsInViewport();
}

void UPriestUIManager::RestoreHUD()
{
    APlayerController* Owner = GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
    JASSERT((IsValid(HUD) && IsValid(Owner)), "%hs [%s]: Cannot restore HUD without a valid HUD and owning PlayerController.", __FUNCTION__, *GetNameSafe(this));
    JASSERT((HUD->GetOwningPlayer() == Owner), "%hs [%s]: Cannot restore a HUD owned by a previous PlayerController.", __FUNCTION__, *GetNameSafe(this));
    if (!HUD->IsInViewport())
    {
        ConnectCombatView();
        JASSERT((HUD->AddToPlayerScreen(0)), "%hs [%s]: Failed to restore HUD to the player screen.", __FUNCTION__, *GetNameSafe(this));
    }
}

void UPriestUIManager::SetMissionState(AIngameGameState* State, AIngamePlayerController* Owner)
{
    JASSERT((IsValid(Owner)), "%hs [%s]: Mission binding requires a valid IngamePlayerController.", __FUNCTION__, *GetNameSafe(this));
    bMissionBindingInitialized = true;
    if (!StageClearController)
    {
        StageClearController = NewObject<UPriestStageClearController>(this);
    }
    StageClearController->Initialize(State, this, Owner);
    if (!MissionController)
    {
        MissionController = NewObject<UPriestMissionController>(this);
    }
    MissionController->SetModel(State);
    MissionController->SetView(HUD);
    MissionController->HandleModelChanged(State, 0);
}

void UPriestUIManager::SetStageClearWidgetClass(TSubclassOf<UPriestStageClearWidget> WidgetClass)
{
    StageClearWidgetClass = WidgetClass;
}

UPriestStageClearWidget* UPriestUIManager::ShowStageClearScreen(APlayerController* Owner)
{
    JASSERT_NULLPTR((IsValid(Owner)), "%hs [%s]: A valid owning PlayerController is required to create the StageClear screen.", __FUNCTION__, *GetNameSafe(this));
    JASSERT_NULLPTR((StageClearWidgetClass && !StageClearWidgetClass->HasAnyClassFlags(CLASS_Abstract)), "%hs [%s]: Configure a concrete StageClearWidgetClass in the ingame PlayerController Blueprint.", __FUNCTION__, *GetNameSafe(this));
    if (IsValid(StageClearWidget) && StageClearWidget->GetOwningPlayer() != Owner)
    {
        HideStageClearScreen();
    }
    if (!IsValid(StageClearWidget))
    {
        StageClearWidget = CreateWidget<UPriestStageClearWidget>(Owner, StageClearWidgetClass);
    }
    JASSERT_NULLPTR((IsValid(StageClearWidget)), "%hs [%s]: CreateWidget failed for StageClearWidgetClass.", __FUNCTION__, *GetNameSafe(this));
    if (!StageClearWidget->HasValidBindings())
    {
        JError("StageClear widget initialization failed");
        HideStageClearScreen();
        return nullptr;
    }
    if (!StageClearWidget->IsInViewport() && !StageClearWidget->AddToPlayerScreen(100))
    {
        JError("%hs [%s]: Failed to add the StageClear screen to the owning player.", __FUNCTION__, *GetNameSafe(this));
        HideStageClearScreen();
        return nullptr;
    }
    StageClearWidget->SetVisibility(ESlateVisibility::Visible);
    return StageClearWidget;
}

void UPriestUIManager::HideStageClearScreen()
{
    if (StageClearWidget)
    {
        StageClearWidget->RemoveFromParent();
    }
    StageClearWidget = nullptr;
}

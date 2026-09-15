#include "PriestUIManager.h"
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
        UE_LOG(LogTemp, Warning, TEXT("Priest HUD: Set a concrete Widget Blueprint class before ShowHUD."));
        return false;
    }
	APlayerController* Controller = GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs [%s]: No PlayerController is available to create the HUD."), __FUNCTION__, *GetNameSafe(this));
	    return false;
	}
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
	if (!HUD)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs [%s]: CreateWidget failed for HUDWidgetClass."), __FUNCTION__, *GetNameSafe(this));
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
            UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Failed to add HUD to the player screen."), __FUNCTION__, *GetNameSafe(this));
        }
        return bDisplayed;
	}
	return true;
}

void UPriestUIManager::UpdateHUD(const FPriestHUDData& Data)
{
    if (!IsValid(HUD))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: HUD is missing. Call ShowHUD before UpdateHUD."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
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
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Failed to display damage number widget."), __FUNCTION__, *GetNameSafe(this));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Failed to create damage number widget."), __FUNCTION__, *GetNameSafe(this));
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
    if (!IsValid(Owner))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Combat binding requires a valid IngamePlayerController."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
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
    if (!IsValid(HUD))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: HUD is missing. Create the HUD before connecting its controllers."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (IsValid(MissionController))
    {
        MissionController->SetView(HUD);
        MissionController->HandleModelChanged(MissionController->GetModel<AIngameGameState>(), 0);
    }
    else if (bMissionBindingInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: MissionController is missing after SetMissionState. Reinitialize mission binding."), __FUNCTION__, *GetNameSafe(this));
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("%hs [%s]: Mission binding has not started; SetMissionState will connect the HUD."), __FUNCTION__, *GetNameSafe(this));
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
            UE_LOG(LogTemp, Error, TEXT("%hs [%s]: CombatController is invalid after SetCombatPawn."), __FUNCTION__, *GetNameSafe(this));
        }
        if (!IsValid(CombatModel))
        {
            UE_LOG(LogTemp, Error, TEXT("%hs [%s]: CombatModel is invalid after SetCombatPawn."), __FUNCTION__, *GetNameSafe(this));
        }
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("%hs [%s]: Combat binding has not started; SetCombatPawn will connect the HUD."), __FUNCTION__, *GetNameSafe(this));
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
    if (!IsValid(Owner))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: A valid owning PlayerController is required to create the Death screen."), __FUNCTION__, *GetNameSafe(this));
        return nullptr;
    }
    if (!DeathWidgetClass || DeathWidgetClass->HasAnyClassFlags(CLASS_Abstract))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Configure a concrete DeathWidgetClass in the ingame PlayerController Blueprint."), __FUNCTION__, *GetNameSafe(this));
        return nullptr;
    }
    if (IsValid(DeathWidget) && DeathWidget->GetOwningPlayer() != Owner)
    {
        HideDeathScreen();
    }
    if (!IsValid(DeathWidget))
    {
        DeathWidget = CreateWidget<UPriestDeathWidget>(Owner, DeathWidgetClass);
    }
    if (!IsValid(DeathWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: CreateWidget failed for DeathWidgetClass."), __FUNCTION__, *GetNameSafe(this));
        return nullptr;
    }
    if (!DeathWidget->IsInViewport() && !DeathWidget->AddToPlayerScreen(100))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Failed to add the Death screen to the owning player."), __FUNCTION__, *GetNameSafe(this));
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
    if (!IsValid(HUD) || !IsValid(Owner))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Cannot restore HUD without a valid HUD and owning PlayerController."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (HUD->GetOwningPlayer() != Owner)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Cannot restore a HUD owned by a previous PlayerController."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    if (!HUD->IsInViewport())
    {
        ConnectCombatView();
        if (!HUD->AddToPlayerScreen(0))
        {
            UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Failed to restore HUD to the player screen."), __FUNCTION__, *GetNameSafe(this));
            return;
        }
    }
}

void UPriestUIManager::SetMissionState(AIngameGameState* State, AIngamePlayerController* Owner)
{
    if (!IsValid(Owner))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Mission binding requires a valid IngamePlayerController."), __FUNCTION__, *GetNameSafe(this));
        return;
    }
    bMissionBindingInitialized = true;
    if (!IsValid(State))
    {
        UE_LOG(LogTemp, Verbose, TEXT("%hs [%s]: Waiting for IngameGameState. Mission view will show loading."), __FUNCTION__, *GetNameSafe(this));
    }
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
    if (!IsValid(Owner))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: A valid owning PlayerController is required to create the StageClear screen."), __FUNCTION__, *GetNameSafe(this));
        return nullptr;
    }
    if (!StageClearWidgetClass || StageClearWidgetClass->HasAnyClassFlags(CLASS_Abstract))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Configure a concrete StageClearWidgetClass in the ingame PlayerController Blueprint."), __FUNCTION__, *GetNameSafe(this));
        return nullptr;
    }
    if (IsValid(StageClearWidget) && StageClearWidget->GetOwningPlayer() != Owner)
    {
        HideStageClearScreen();
    }
    if (!IsValid(StageClearWidget))
    {
        StageClearWidget = CreateWidget<UPriestStageClearWidget>(Owner, StageClearWidgetClass);
    }
    if (!IsValid(StageClearWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: CreateWidget failed for StageClearWidgetClass."), __FUNCTION__, *GetNameSafe(this));
        return nullptr;
    }
    if (!StageClearWidget->IsInViewport() && !StageClearWidget->AddToPlayerScreen(100))
    {
        UE_LOG(LogTemp, Error, TEXT("%hs [%s]: Failed to add the StageClear screen to the owning player."), __FUNCTION__, *GetNameSafe(this));
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

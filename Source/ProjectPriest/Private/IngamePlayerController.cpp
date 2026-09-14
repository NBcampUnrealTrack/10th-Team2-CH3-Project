#include "IngamePlayerController.h"
#include "PlayerCharacter.h"
#include "WeaponItem.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "../UI/PriestHUDWidget.h"
#include "../UI/PriestUIManager.h"

AIngamePlayerController::AIngamePlayerController()
{
}

void AIngamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer)
    {
        return;
    }

    if (HUDWidgetClass)
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->SetHUDWidgetClass(HUDWidgetClass);
            UI->ShowHUD(InitialHUDData);
        }
    }

    OnPossessedPawnChanged.AddDynamic(this, &AIngamePlayerController::HandleCombatPawnChanged);
    HandleCombatPawnChanged(nullptr, GetPawn());

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    //JASSERT(IsValid(Subsystem), "UEnhancedInputLocalPlayerSubsystem Not exist");
    //JASSERT(IsValid(InputMappingContext), "Input mapping context not exist");

    if (Subsystem && InputMappingContext)
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }
}

TObjectPtr<UInputAction> AIngamePlayerController::GetMoveAction()
{
    return MoveAction;
}

TObjectPtr<UInputAction> AIngamePlayerController::GetLookAction()
{
    return LookAction;
}

TObjectPtr<UInputAction> AIngamePlayerController::GetFireAction()
{
    return FireAction;
}

TObjectPtr<UInputAction> AIngamePlayerController::GetThrowAction()
{
    return ThrowAction;
}

TObjectPtr<UInputAction> AIngamePlayerController::GetSprintAction()
{
    return SprintAction;
}

TObjectPtr<UInputAction> AIngamePlayerController::GetJumpAction()
{
    return JumpAction;
}

TObjectPtr<UInputAction> AIngamePlayerController::GetInteractAction()
{
    return InteractAction;
}

void AIngamePlayerController::UnbindCombatHUD()
{
    if (HUDPlayer.IsValid())
    {
        HUDPlayer->OnCombatChanged.Remove(HealthChangedHandle);
    }
    if (HUDWeapon.IsValid())
    {
        HUDWeapon->OnAmmoChanged.Remove(AmmoChangedHandle);
    }
    HUDPlayer.Reset();
    HUDWeapon.Reset();
    HealthChangedHandle.Reset();
    AmmoChangedHandle.Reset();
}

void AIngamePlayerController::HandleCombatPawnChanged(APawn* PreviousPawn, APawn* NewPawn)
{
    UnbindCombatHUD();
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->ResetDamageFeedback();
        }
    }
    HUDPlayer = Cast<APlayerCharacter>(NewPawn);
    if (HUDPlayer.IsValid())
    {
        HealthChangedHandle = HUDPlayer->OnCombatChanged.AddUObject(this, &AIngamePlayerController::RefreshCombatHUD);
    }
    RefreshCombatHUD();
}

void AIngamePlayerController::RefreshCombatHUD()
{
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer)
    {
        return;
    }
    UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>();
    if (!UI)
    {
        return;
    }

    APlayerCharacter* CombatCharacter = HUDPlayer.Get();
    AWeaponItem* Weapon = CombatCharacter ? CombatCharacter->GetEquippedWeapon() : nullptr;
    if (!IsValid(Weapon))
    {
        Weapon = nullptr;
    }
    if (HUDWeapon.Get() != Weapon)
    {
        if (HUDWeapon.IsValid())
        {
            HUDWeapon->OnAmmoChanged.Remove(AmmoChangedHandle);
        }
        HUDWeapon = Weapon;
        AmmoChangedHandle.Reset();
        if (Weapon)
        {
            AmmoChangedHandle = Weapon->OnAmmoChanged.AddUObject(this, &AIngamePlayerController::RefreshCombatHUD);
        }
    }
    UI->UpdateCombatHUD(
        CombatCharacter ? CombatCharacter->GetCurrentHealth() : 0.0f,
        CombatCharacter ? CombatCharacter->GetMaxHealth() : 0.0f,
        Weapon ? Weapon->GetWeaponName() : NSLOCTEXT("PriestHUD", "NoWeapon", "Unarmed"),
        Weapon ? Weapon->GetCurrentAmmo() : 0,
        Weapon ? Weapon->GetReserveAmmo() : 0);
}

void AIngamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    OnPossessedPawnChanged.RemoveDynamic(this, &AIngamePlayerController::HandleCombatPawnChanged);
    UnbindCombatHUD();
    Super::EndPlay(EndPlayReason);
}

void AIngamePlayerController::ClientNotifyHitConfirmed_Implementation(float AppliedDamage, FVector DamageLocation)
{
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->NotifyHitConfirmed(AppliedDamage, DamageLocation);
        }
    }
}

void AIngamePlayerController::ClientNotifyPlayerDamaged_Implementation(APawn* DamagedPawn)
{
    if (!DamagedPawn || DamagedPawn != GetPawn())
    {
        return;
    }
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->NotifyPlayerDamaged();
        }
    }
}

void AIngamePlayerController::ClientNotifyEnemyKilled_Implementation()
{
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->NotifyEnemyKilled();
        }
    }
}

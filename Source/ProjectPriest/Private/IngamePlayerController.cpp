#include "IngamePlayerController.h"
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

    // 메뉴의 UI 전용 입력을 전투 입력으로 복원한다.
    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;

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

void AIngamePlayerController::HandleCombatPawnChanged(APawn* PreviousPawn, APawn* NewPawn)
{
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->SetCombatPawn(NewPawn);
        }
    }
}
void AIngamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    OnPossessedPawnChanged.RemoveDynamic(this, &AIngamePlayerController::HandleCombatPawnChanged);
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->DisconnectCombatHUD();
        }
    }
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

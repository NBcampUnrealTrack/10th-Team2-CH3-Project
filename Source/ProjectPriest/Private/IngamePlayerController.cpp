#include "IngamePlayerController.h"
#include "../UI/PriestDeathWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "../UI/PriestHUDWidget.h"
#include "../UI/PriestUIManager.h"

AIngamePlayerController::AIngamePlayerController()
{
    MainMenuMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/01_PP/Levels/L_MainMenu.L_MainMenu")));
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

    if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
    {
        UI->SetDeathWidgetClass(DeathWidgetClass);
    }
    if (GEngine)
    {
        DeathTravelFailureHandle = GEngine->OnTravelFailure().AddUObject(this, &AIngamePlayerController::HandleDeathTravelFailure);
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

void AIngamePlayerController::HandleCombatPawnChanged(APawn* PreviousPawn, APawn* NewPawn)
{
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->SetCombatPawn(NewPawn, this);
        }
    }
}
void AIngamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GEngine) GEngine->OnTravelFailure().Remove(DeathTravelFailureHandle);
    DeathTravelFailureHandle.Reset();
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

void AIngamePlayerController::SetDeathInput(bool bActive, UPriestDeathWidget* Widget)
{
    if (!IsLocalController()) return;
    if (bDeathInputActive != bActive)
    {
        SetIgnoreMoveInput(bActive);
        SetIgnoreLookInput(bActive);
        bDeathInputActive = bActive;
    }
    if (PlayerInput) PlayerInput->FlushPressedKeys();
    bShowMouseCursor = bActive;
    if (bActive)
    {
        if (GetPawn() && GetPawn()->GetMovementComponent()) GetPawn()->GetMovementComponent()->StopMovementImmediately();
        FInputModeUIOnly Mode;
        if (Widget) Mode.SetWidgetToFocus(Widget->TakeWidget());
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(Mode);
    }
    else
    {
        bDeathTravelRequested = false;
        SetInputMode(FInputModeGameOnly());
    }
}

bool AIngamePlayerController::ExecuteDeathTravel(bool bRestart, FText& OutError)
{
    OutError = FText::GetEmpty();
    // This is the engine-side executor. Death eligibility and view updates belong to MVC.
    if (!IsLocalController() || !bDeathInputActive || bDeathTravelRequested || !GetWorld())
    {
        OutError = NSLOCTEXT("PriestDeath", "Unavailable", "현재 맵 이동을 요청할 수 없습니다.");
        return false;
    }
    if (GetNetMode() != NM_Standalone)
    {
        OutError = NSLOCTEXT("PriestDeath", "LocalOnly", "현재 재시작은 싱글 플레이에서만 지원합니다.");
        return false;
    }
    // Keep the full asset path for package lookup; PIE only changes the map name prefix.
    const FString Package = bRestart
        ? UWorld::RemovePIEPrefix(GetWorld()->GetOutermost()->GetName())
        : MainMenuMap.ToSoftObjectPath().GetLongPackageName();
    if (Package.IsEmpty() || !FPackageName::DoesPackageExist(Package))
    {
        OutError = NSLOCTEXT("PriestDeath", "MissingMap", "이동할 맵을 찾을 수 없습니다. 맵 설정을 확인해 주세요.");
        UE_LOG(LogTemp, Warning, TEXT("Priest Death UI: Map does not exist: %s"), *Package);
        return false;
    }
    bDeathTravelRequested = true;
    UGameplayStatics::OpenLevel(this, FName(*Package));
    return true;
}

void AIngamePlayerController::HandleDeathTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
    if (World != GetWorld() || !bDeathTravelRequested) return;
    bDeathTravelRequested = false;
    UE_LOG(LogTemp, Warning, TEXT("Priest Death UI: Travel failed (%d): %s"), static_cast<int32>(FailureType), *ErrorString);
    OnDeathTravelFailed.Broadcast(NSLOCTEXT("PriestDeath", "TravelFailed", "맵 이동에 실패했습니다. 다시 시도해 주세요."));
}

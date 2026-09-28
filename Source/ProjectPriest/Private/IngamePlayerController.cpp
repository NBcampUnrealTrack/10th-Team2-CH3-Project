#include "IngamePlayerController.h"
#include "JUtility.h"
#include "IngameGameState.h"
#include "../UI/PriestStageClearWidget.h"
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
        UI->SetStageClearWidgetClass(StageClearWidgetClass);
    }
    else
    {
        JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
    }
    if (GEngine)
    {
        ResultTravelFailureHandle = GEngine->OnTravelFailure().AddUObject(this, &AIngamePlayerController::HandleResultTravelFailure);
    }

    if (HUDWidgetClass)
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->SetHUDWidgetClass(HUDWidgetClass);
            UI->ShowHUD(InitialHUDData);
        }
        else
        {
            JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
        }
    }

    MissionGameStateHandle = GetWorld()->GameStateSetEvent.AddUObject(this, &AIngamePlayerController::HandleMissionGameStateChanged);
    HandleMissionGameStateChanged(GetWorld()->GetGameState());

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
        else
        {
            JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
        }
    }
}
void AIngamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GEngine)
    {
        GEngine->OnTravelFailure().Remove(ResultTravelFailureHandle);
    }
    ResultTravelFailureHandle.Reset();
    if (GetWorld())
    {
        GetWorld()->GameStateSetEvent.Remove(MissionGameStateHandle);
    }
    MissionGameStateHandle.Reset();
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
        else
        {
            JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
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
        else
        {
            JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
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
        else
        {
            JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
        }
    }
}

void AIngamePlayerController::SetResultInput(bool bActive, UUserWidget* Widget)
{
    if (!IsLocalController())
    {
        return;
    }
    if (bResultInputActive != bActive)
    {
        SetIgnoreMoveInput(bActive);
        SetIgnoreLookInput(bActive);
        bResultInputActive = bActive;
    }
    if (PlayerInput)
    {
        PlayerInput->FlushPressedKeys();
    }
    bShowMouseCursor = bActive;
    if (bActive)
    {
        if (GetPawn() && GetPawn()->GetMovementComponent())
        {
            GetPawn()->GetMovementComponent()->StopMovementImmediately();
        }
        FInputModeUIOnly Mode;
        if (Widget)
        {
            Mode.SetWidgetToFocus(Widget->TakeWidget());
        }
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(Mode);
    }
    else
    {
        bResultTravelRequested = false;
        SetInputMode(FInputModeGameOnly());
    }
}

bool AIngamePlayerController::ExecuteResultTravel(bool bRestart, FText& OutError)
{
    OutError = FText::GetEmpty();
    // This is the engine-side executor. Result eligibility and view updates belong to MVC.
    if (!IsLocalController() || !bResultInputActive || bResultTravelRequested || !GetWorld())
    {
        OutError = NSLOCTEXT("PriestDeath", "Unavailable", "Map travel is currently unavailable.");
        return false;
    }
    if (GetNetMode() != NM_Standalone)
    {
        OutError = NSLOCTEXT("PriestDeath", "LocalOnly", "Map travel is only supported in single player.");
        return false;
    }
    // Keep the full asset path for package lookup; PIE only changes the map name prefix.
    const FString Package = bRestart
        ? UWorld::RemovePIEPrefix(GetWorld()->GetOutermost()->GetName())
        : MainMenuMap.ToSoftObjectPath().GetLongPackageName();
    if (Package.IsEmpty() || !FPackageName::DoesPackageExist(Package))
    {
        OutError = NSLOCTEXT("PriestDeath", "MissingMap", "Map not found. Check the map settings.");
        JWarning("Priest Death UI: Map does not exist: %s", *Package);
        return false;
    }
    bResultTravelRequested = true;
    const FString Options = bRestart ? TEXT("") : TEXT("MenuTab=Equipment");
    UGameplayStatics::OpenLevel(this, FName(*Package), true, Options);
    return true;
}

void AIngamePlayerController::HandleResultTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
    if (World != GetWorld() || !bResultTravelRequested)
    {
        return;
    }
    bResultTravelRequested = false;
    JWarning("Priest Death UI: Travel failed (%d): %s", static_cast<int32>(FailureType), *ErrorString);
    OnResultTravelFailed.Broadcast(NSLOCTEXT("PriestDeath", "TravelFailed", "Map travel failed. Please try again."));
}

void AIngamePlayerController::HandleMissionGameStateChanged(AGameStateBase* State)
{
    if (IsValid(State) && !Cast<AIngameGameState>(State))
    {
        JError("%hs [%s]: GameState has the wrong type. Set GameStateClass to IngameGameState in the game mode.", __FUNCTION__, *GetNameSafe(this));
    }
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->SetMissionState(Cast<AIngameGameState>(State), this);
        }
        else
        {
            JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
        }
    }
}

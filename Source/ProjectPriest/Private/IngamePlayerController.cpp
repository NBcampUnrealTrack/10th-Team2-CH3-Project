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
    if (!LocalPlayer) return;

    if (HUDWidgetClass)
    {
        if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
        {
            UI->SetHUDWidgetClass(HUDWidgetClass);
            UI->ShowHUD(InitialHUDData);
        }
    }

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

#include "PriestMainMenuPlayerController.h"
#include "JUtility.h"
#include "PriestMainMenuWidget.h"
#include "PriestMenuMvc.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PriestUIManager.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PackageName.h"

APriestMainMenuPlayerController::APriestMainMenuPlayerController()
{
	StageOneMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/ElderboomVillage/Maps/L_ElderboomVillage.L_ElderboomVillage")));
}

void APriestMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	if (UPriestUIManager* UI = LocalPlayer->GetSubsystem<UPriestUIManager>())
	{
		UI->DisconnectCombatHUD();
	}
	else
	{
		JError("%hs [%s]: PriestUIManager subsystem is missing. UI request cannot be processed.", __FUNCTION__, *GetNameSafe(this));
	}
	JASSERT((MainMenuWidgetClass && !MainMenuWidgetClass->HasAnyClassFlags(CLASS_Abstract)), "Priest Menu: Set MainMenuWidgetClass in the menu PlayerController Blueprint.");

	MainMenu = CreateWidget<UPriestMainMenuWidget>(this, MainMenuWidgetClass);
    JASSERT((IsValid(MainMenu)), "Could not create MainMenuWidgetClass");
    if (!MainMenu->HasValidBindings())
    {
        JError("Main menu widget initialization failed");
        MainMenu = nullptr;
        return;
    }
    MenuModel = NewObject<UPriestMenuModel>(this);
    MenuController = NewObject<UPriestMenuController>(this);
    MenuController->SetModel(MenuModel);
    MenuController->SetView(MainMenu);
    JASSERT((MainMenu->AddToPlayerScreen(0)), "Could not display the main menu");

    MenuController->HandleModelChanged(MenuModel, 0);
	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	if (GEngine)
	{
		TravelFailureHandle = GEngine->OnTravelFailure().AddUObject(this, &APriestMainMenuPlayerController::HandleTravelFailure);
	}
}

bool APriestMainMenuPlayerController::StartStageOne()
{
	if (!IsLocalController() || !MainMenu || bTravelRequested)
	{
		return false;
	}

	const FString MapPackage = StageOneMap.ToSoftObjectPath().GetLongPackageName();
	if (StageOneMap.IsNull() || !FPackageName::DoesPackageExist(MapPackage))
	{
		JError("Priest Menu: StageOneMap does not exist: %s", *MapPackage);
		MainMenu->ShowStatusMessage(NSLOCTEXT("PriestMenu", "MissingStage", "게임플레이 맵을 찾을 수 없습니다."));
		return false;
	}

	bTravelRequested = true;
	MainMenu->SetIsEnabled(false);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, StageOneMap);
	return true;
}

void APriestMainMenuPlayerController::HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	if (World != GetWorld() || !bTravelRequested)
	{
		return;
	}

	bTravelRequested = false;
	JError("Priest Menu: Travel failed (%d): %s", static_cast<int32>(FailureType), *ErrorString);
	if (MainMenu)
	{
		MainMenu->SetIsEnabled(true);
		MainMenu->ShowStatusMessage(NSLOCTEXT("PriestMenu", "TravelFailed", "맵 이동에 실패했습니다. 다시 시도해 주세요."));
	}
}

void APriestMainMenuPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GEngine)
	{
		GEngine->OnTravelFailure().Remove(TravelFailureHandle);
	}
	TravelFailureHandle.Reset();
    if (MenuController)
    {
        MenuController->Disconnect();
    }
    MenuController = nullptr;
    MenuModel = nullptr;
	if (MainMenu)
	{
		MainMenu->RemoveFromParent();
		MainMenu = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void APriestMainMenuPlayerController::RequestQuitGame()
{
    if (CanProcessMenuRequest())
    {
        UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
    }
}

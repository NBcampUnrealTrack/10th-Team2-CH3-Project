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
#include "PriestInventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "PriestInventoryModel.h"
#include "PriestInventoryController.h"

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
	JASSERT(
		(MainMenuWidgetClass && !MainMenuWidgetClass->HasAnyClassFlags(CLASS_Abstract)),
		"Priest Menu: Set MainMenuWidgetClass in the menu PlayerController Blueprint."
	);

	UGameInstance* Instance = GetGameInstance();

	JASSERT(
        IsValid(Instance),
		"%hs: GameInstance가 없습니다.",
		__FUNCTION__
	);

	UPriestInventorySubsystem* Inventory =
		Instance->GetSubsystem<UPriestInventorySubsystem>();

	JASSERT(
        IsValid(Inventory),
		"%hs: InventorySubsystem 연결에 실패했습니다.",
		__FUNCTION__
	);

	if (bGrantPreviewInventory)
	{
		Inventory->GrantPreviewItemsOnce();
	}

	MainMenu = CreateWidget<UPriestMainMenuWidget>(this, MainMenuWidgetClass);
	JASSERT(IsValid(MainMenu), "%hs: MainMenu 생성 실패", __FUNCTION__);

	if (!MainMenu->HasValidBindings())
	{
		JError("%hs: MainMenu 필수 위젯 바인딩 확인 실패", __FUNCTION__);

		MainMenu = nullptr;
		return;
	}

	MenuModel = NewObject<UPriestMenuModel>(this);
	JASSERT(IsValid(MenuModel), "%hs: MenuModel 생성 실패", __FUNCTION__);

	MenuController = NewObject<UPriestMenuController>(this);
	JASSERT(IsValid(MenuController), "%hs: MenuController 생성 실패", __FUNCTION__);

	InventoryModel = NewObject<UPriestInventoryModel>(this);
	JASSERT(IsValid(InventoryModel), "%hs: InventoryModel 생성 실패", __FUNCTION__);

	InventoryController = NewObject<UPriestInventoryController>(this);
	JASSERT(IsValid(InventoryController), "%hs: InventoryController 생성 실패", __FUNCTION__);

	InventoryModel->Initialize(Inventory);

	MenuController->SetModel(MenuModel);
	MenuController->SetView(MainMenu);

	InventoryController->SetModel(InventoryModel);
	InventoryController->SetView(MainMenu);

	JASSERT(MainMenu->AddToPlayerScreen(0), "%hs: MainMenu 화면 표시 실패", __FUNCTION__);

	MenuController->HandleModelChanged(MenuModel, 0);
	InventoryController->HandleModelChanged(InventoryModel, 0);

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
	if (InventoryController)
	{
		InventoryController->Disconnect();
	}

	if (InventoryModel)
	{
		InventoryModel->Disconnect();
	}

	InventoryController = nullptr;
	InventoryModel = nullptr;
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

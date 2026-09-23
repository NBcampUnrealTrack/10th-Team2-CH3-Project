#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/PlayerController.h"
#include "PriestMainMenuPlayerController.generated.h"

class UPriestMainMenuWidget;
class UPriestMenuModel;
class UPriestMenuController;
class UPriestInventoryModel;
class UPriestInventoryController;

UCLASS()
class PROJECTPRIEST_API APriestMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APriestMainMenuPlayerController();
	void StartStage(int32 StageIndex);
    bool CanProcessMenuRequest() const
    {
        return IsLocalController() && !bTravelRequested;
    }
    void RequestQuitGame();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);

protected:
	// Temporary inventory preview. Disable when crafting/rewards supply real items.
	UPROPERTY(EditDefaultsOnly, Category = "Priest|Inventory")
	bool bGrantPreviewInventory = true;

	UPROPERTY(EditDefaultsOnly, Category = "Priest|Menu")
	TSubclassOf<UPriestMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Priest|Menu")
	TArray<TSoftObjectPtr<UWorld>> StageMaps;

private:
    UPROPERTY(Transient) TObjectPtr<UPriestMenuModel> MenuModel;
    UPROPERTY(Transient) TObjectPtr<UPriestMenuController> MenuController;

	UPROPERTY(Transient) TObjectPtr<UPriestMainMenuWidget> MainMenu;
	UPROPERTY(Transient) TObjectPtr<UPriestInventoryModel> InventoryModel;

	UPROPERTY(Transient) TObjectPtr<UPriestInventoryController> InventoryController;

	FDelegateHandle TravelFailureHandle;
	bool bTravelRequested = false;
};

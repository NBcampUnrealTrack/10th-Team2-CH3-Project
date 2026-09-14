#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/PlayerController.h"
#include "PriestMainMenuPlayerController.generated.h"

class UPriestMainMenuWidget;

UCLASS()
class PROJECTPRIEST_API APriestMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APriestMainMenuPlayerController();
	bool StartStageOne();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Priest|Menu")
	TSubclassOf<UPriestMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Priest|Menu")
	TSoftObjectPtr<UWorld> StageOneMap;

private:
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);

	UPROPERTY(Transient)
	TObjectPtr<UPriestMainMenuWidget> MainMenu;

	FDelegateHandle TravelFailureHandle;
	bool bTravelRequested = false;
};

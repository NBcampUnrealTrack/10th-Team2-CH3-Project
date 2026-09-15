#pragma once
#include "CoreMinimal.h"
#include "MvcControl.h"
#include "PriestStageClearController.generated.h"
class AIngameGameState;
class UPriestUIManager;
class AIngamePlayerController;

UCLASS()
class PROJECTPRIEST_API UPriestStageClearController : public UMvcControl
{
    GENERATED_BODY()
public:
    // Dependencies are supplied by the composition root, independent of UObject Outer.
    void Initialize(AIngameGameState* Model, UPriestUIManager* UI, AIngamePlayerController* PlayerController);
    virtual void Disconnect() override;
    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) override;
    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) override;
    bool IsStageClearActive() const
    {
        return bStageClearActive;
    }
private:
    void EnterStageClear();
    void LeaveStageClear(bool bRestorePreviousHUD);
    void HandleTravelFailed(const FText& Message);
    void PresentTravelStatus(bool bBusy, const FText& Message);
    TWeakObjectPtr<UPriestUIManager> UIManager;
    TWeakObjectPtr<AIngamePlayerController> PlayerController;
    FDelegateHandle TravelFailedHandle;
    bool bStageClearActive = false;
    bool bRestoreHUD = false;
    bool bTravelPending = false;
};

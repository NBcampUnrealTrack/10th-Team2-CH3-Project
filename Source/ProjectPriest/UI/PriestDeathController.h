#pragma once
#include "CoreMinimal.h"
#include "MvcControl.h"
#include "PriestDeathController.generated.h"
class UPriestCombatModel;
class UPriestUIManager;
class AIngamePlayerController;

UCLASS()
class PROJECTPRIEST_API UPriestDeathController : public UMvcControl
{
    GENERATED_BODY()
public:
    // Dependencies are supplied by the composition root, independent of UObject Outer.
    void Initialize(UPriestCombatModel* Model, UPriestUIManager* UI, AIngamePlayerController* PlayerController);
    virtual void Disconnect() override;
    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) override;
    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) override;
    bool IsDeathActive() const
    {
        return bDeathActive;
    }
private:
    void EnterDeath();
    void LeaveDeath(bool bRestorePreviousHUD);
    void HandleTravelFailed(const FText& Message);
    void PresentTravelStatus(bool bBusy, const FText& Message);
    TWeakObjectPtr<UPriestUIManager> UIManager;
    TWeakObjectPtr<AIngamePlayerController> PlayerController;
    FDelegateHandle TravelFailedHandle;
    bool bDeathActive = false;
    bool bRestoreHUD = false;
    bool bTravelPending = false;
};

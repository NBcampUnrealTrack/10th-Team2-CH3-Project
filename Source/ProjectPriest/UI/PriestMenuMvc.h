#pragma once
#include "CoreMinimal.h"
#include "MvcModel.h"
#include "MvcControl.h"
#include "PriestMainMenuWidget.h"
#include "PriestMenuMvc.generated.h"

UENUM()
enum class EPriestMenuAction : uint8 { Title, Regions, Equipment, Credits, SwitchTab, StartStage, Quit };

UCLASS()
class PROJECTPRIEST_API UPriestMenuRequest : public UEventParameterBase
{
    GENERATED_BODY()
public:
    UPriestMenuRequest() { EventType = EViewEventType::ButtonClicked; }
    EPriestMenuAction Action = EPriestMenuAction::Title;
    bool bAccepted = false;
};

UCLASS()
class PROJECTPRIEST_API UPriestMenuModel : public UObject, public IMvcModel
{
    GENERATED_BODY()
public:
    EPriestMenuPage GetPage() const { return Page; }
    EPriestLobbyTab GetTab() const { return Tab; }
    void SetState(EPriestMenuPage NewPage, EPriestLobbyTab NewTab);
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokePropertyChanged(uint8 PropertyName) override;
private:
    EPriestMenuPage Page = EPriestMenuPage::Title;
    EPriestLobbyTab Tab = EPriestLobbyTab::Region;
    FModelChangedDelegate Changed;
};

UCLASS()
class PROJECTPRIEST_API UPriestMenuController : public UMvcControl
{
    GENERATED_BODY()
public:
    virtual void HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter) override;
    virtual void HandleModelChanged(IMvcModel* InModel, uint8 PropertyName) override;
};

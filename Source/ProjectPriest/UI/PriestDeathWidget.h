#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "PriestDeathWidget.generated.h"
class UButton;
class UTextBlock;

UCLASS()
class PROJECTPRIEST_API UPriestDeathRequest : public UEventParameterBase
{
    GENERATED_BODY()
public:
    UPriestDeathRequest() { EventType = EViewEventType::ButtonClicked; }
    bool bRestart = true;
};

// Layout, labels and button styles belong to the child Widget Blueprint.
UCLASS(Abstract)
class PROJECTPRIEST_API UPriestDeathWidget : public UUserWidget, public IMvcView
{
    GENERATED_BODY()
public:
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter) override;
    void SetBusy(bool bBusy);
    void ShowStatus(const FText& Message);
    void FocusRestart();
protected:
    virtual void NativeOnInitialized() override;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> RestartButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> MainMenuButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> StatusText;
private:
    UFUNCTION() void RequestRestart();
    UFUNCTION() void RequestMainMenu();
    void SendRequest(bool bRestart);
    FViewEventRaisedDelegate Listener;
};

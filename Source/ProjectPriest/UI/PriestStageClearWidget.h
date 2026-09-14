#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "PriestStageClearWidget.generated.h"
class UButton;
class UTextBlock;

UCLASS()
class PROJECTPRIEST_API UPriestStageClearRequest : public UEventParameterBase
{
    GENERATED_BODY()
public:
    UPriestStageClearRequest()
    {
        EventType = EViewEventType::ButtonClicked;
    }
};

// Layout, labels and button styles belong to the child Widget Blueprint.
UCLASS(Abstract)
class PROJECTPRIEST_API UPriestStageClearWidget : public UUserWidget, public IMvcView
{
    GENERATED_BODY()
public:
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter) override;
    static FText FormatClearTime(float Seconds);
    void SetClearTime(float Seconds);
    void SetBusy(bool bBusy);
    void ShowStatus(const FText& Message);
    void FocusMainMenu();
protected:
    virtual void NativeOnInitialized() override;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> ClearTimeText;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> MainMenuButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> StatusText;
private:
    UFUNCTION() void RequestMainMenu();
    FViewEventRaisedDelegate Listener;
};

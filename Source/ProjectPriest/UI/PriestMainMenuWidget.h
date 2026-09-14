#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "PriestMainMenuWidget.generated.h"

enum class EPriestMenuAction : uint8;
class UTextBlock;
class UWidgetSwitcher;

// 값은 WBP의 MenuSwitcher 자식 순서와 일치해야 한다.
UENUM(BlueprintType)
enum class EPriestMenuPage : uint8
{
	Title,
	Lobby,
	Credits
};

// 값은 WBP의 LobbySwitcher 자식 순서와 일치해야 한다.
UENUM(BlueprintType)
enum class EPriestLobbyTab : uint8
{
	Region,
	Equipment
};

// 배치, 버튼 스타일, 문구는 WBP Designer에서 구성한다.
UCLASS(Abstract)
class PROJECTPRIEST_API UPriestMainMenuWidget : public UUserWidget, public IMvcView
{
	GENERATED_BODY()

public:
    virtual bool Initialize() override;
    bool HasValidBindings() const
    {
        return bBindingsReady;
    }
    void ApplyMenuState(EPriestMenuPage Page, EPriestLobbyTab Tab);
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter) override;
	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	void ShowTitle();

	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	void ShowRegions();

	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	void ShowEquipment();

	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	void ShowCredits();

	// 현재 구현된 두 탭 사이를 순환한다. 좌우 화살표에서 호출한다.
	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	void SwitchLobbyTab();

	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	bool StartStageOne();

	UFUNCTION(BlueprintCallable, Category = "Priest|Menu")
	void QuitGame();

	void ShowStatusMessage(const FText& Message);

protected:
	virtual void NativeConstruct() override;

	// 선택 탭의 색상 변경이나 버튼 포커스는 WBP에서 필요에 따라 구현한다.
	UFUNCTION(BlueprintImplementableEvent, Category = "Priest|Menu")
	void OnMenuStateChanged(EPriestMenuPage Page, EPriestLobbyTab Tab);

	UPROPERTY(BlueprintReadOnly, Category = "Priest|Menu")
	EPriestMenuPage CurrentPage = EPriestMenuPage::Title;

	UPROPERTY(BlueprintReadOnly, Category = "Priest|Menu")
	EPriestLobbyTab CurrentTab = EPriestLobbyTab::Region;

private:
    bool bBindingsReady = false;
    bool SendRequest(EPriestMenuAction Action);
    FViewEventRaisedDelegate Listener;
	void RefreshPage();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> LobbySwitcher;

	// 필요하면 루트 Canvas에 추가하여 맵 이동 오류를 표시한다.
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> MenuStatusText;
};

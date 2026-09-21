#include "PriestMainMenuWidget.h"
#include "JUtility.h"
#include "MvcControl.h"
#include "Engine/Engine.h"
#include "PriestMainMenuPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "PriestMenuRequest.h"
#include "UObject/StrongObjectPtr.h"
#include "PriestInventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "PriestInventoryEventParameter.h"

#define REQUIRED_MENU_COUNT 3
#define REQUIRED_LOBBY_COUNT 3

bool UPriestMainMenuWidget::Initialize()
{
    if (bBindingsReady)
    {
        return true;
    }
    JASSERT_BOOL((Super::Initialize()), "Base widget initialization failed");
    JASSERT_BOOL((IsValid(MenuSwitcher)), "Missing required MenuSwitcher binding");
    JASSERT_BOOL((IsValid(LobbySwitcher)), "Missing required LobbySwitcher binding");
    JASSERT_BOOL((MenuSwitcher->GetNumWidgets() == REQUIRED_MENU_COUNT), "MenuSwitcher requires Title, Lobby and Credits pages");
    JASSERT_BOOL((LobbySwitcher->GetNumWidgets() == REQUIRED_LOBBY_COUNT), "LobbySwitcher requires Region, Equipment and Crafting pages");
    bBindingsReady = true;
    return true;
}

void UPriestMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
    if (!bBindingsReady)
    {
        return;
    }
	SetIsFocusable(true);
	ShowStatusMessage(FText::GetEmpty());
	RefreshPage();
    if (bHasInventoryData)
    {
        RefreshInventoryDisplay();
    }

}

void UPriestMainMenuWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UPriestMainMenuWidget::SetInventoryData(const FPriestInventoryViewData& InData)
{
    InventoryData = InData;
    bHasInventoryData = true;
    RefreshInventoryDisplay();
}

void UPriestMainMenuWidget::SelectInventoryCategory(EItemCategory InCategory)
{
    TStrongObjectPtr<UPriestInventoryEventParameter> Request(
        NewObject<UPriestInventoryEventParameter>()
    );

    Request->Action = EPriestInventoryAction::SelectCategory;

    Request->Category = InCategory;

    InvokeViewEvent(EViewEventType::InventoryRequest, Request.Get());
}

void UPriestMainMenuWidget::RefreshInventoryDisplay()
{
    const TArray<FPriestInventorySlotData>& Items =
        InventoryData.Items;

    JASSERT(IsValid(InventoryGrid),
        "%hs [%s]: InventoryGrid 바인딩이 없습니다. WBP의 Uniform Grid Panel 이름을 확인하세요.",
        __FUNCTION__,
        *GetNameSafe(this)
    );

    InventoryGrid->ClearChildren();

    if (!InventorySlotClass
        || InventorySlotClass->HasAnyClassFlags(CLASS_Abstract))
    {
        JWarning("Main menu: Set InventorySlotClass " "to WBP_InventorySlot in Class Defaults.");
        return;
    }

    const int32 Columns = FMath::Max(1, InventoryColumns);

    for (int32 Index = 0; Index < Items.Num(); ++Index)
    {
        UPriestInventorySlotWidget* SlotWidget =
            CreateWidget<UPriestInventorySlotWidget>(
                GetOwningPlayer(),
                InventorySlotClass
            );

        if (!IsValid(SlotWidget))
        {
            JError("%hs: 슬롯 생성 실패. ItemId=%s", __FUNCTION__, *Items[Index].ItemId.ToString());
            continue;
        }

        SlotWidget->SetItem(Items[Index]);

        InventoryGrid->AddChildToUniformGrid(
            SlotWidget,
            Index / Columns,
            Index % Columns
        );
    }
}

void UPriestMainMenuWidget::ShowTitle()
{
    SendRequest(EPriestMenuAction::Title);
}
void UPriestMainMenuWidget::ShowRegions()
{
    SendRequest(EPriestMenuAction::Regions);
}
void UPriestMainMenuWidget::ShowEquipment()
{
    SendRequest(EPriestMenuAction::Equipment);
}
void UPriestMainMenuWidget::ShowCrafting()
{
    SendRequest(EPriestMenuAction::Crafting);
}
void UPriestMainMenuWidget::ShowCredits()
{
    SendRequest(EPriestMenuAction::Credits);
}
void UPriestMainMenuWidget::SwitchLobbyTab(int32 Direction)
{
    SendRequest(EPriestMenuAction::SwitchTab, Direction);
}
void UPriestMainMenuWidget::ApplyMenuState(EPriestMenuPage Page, EPriestLobbyTab Tab)
{
    CurrentPage = Page;
    CurrentTab = Tab;
    RefreshPage();
}
void UPriestMainMenuWidget::RefreshPage()
{
	MenuSwitcher->SetActiveWidgetIndex(static_cast<int32>(CurrentPage));
	LobbySwitcher->SetActiveWidgetIndex(static_cast<int32>(CurrentTab));
	ShowStatusMessage(FText::GetEmpty());
	OnMenuStateChanged(CurrentPage, CurrentTab);
}

bool UPriestMainMenuWidget::StartStageOne()
{
    return SendRequest(EPriestMenuAction::StartStage);
}
void UPriestMainMenuWidget::QuitGame()
{
    SendRequest(EPriestMenuAction::Quit);
}
bool UPriestMainMenuWidget::SendRequest(EPriestMenuAction Action, int32 TabDirection)
{
    TStrongObjectPtr<UPriestMenuRequest> Request(NewObject<UPriestMenuRequest>());
    Request->Action = Action;
    Request->TabDirection = TabDirection;
    InvokeViewEvent(EViewEventType::ButtonClicked, Request.Get());
    return Request->bAccepted;
}
FDelegateHandle UPriestMainMenuWidget::AddListener(UMvcControl* Control)
{
    return Listener.AddUObject(Control, &UMvcControl::HandleViewEvent);
}
void UPriestMainMenuWidget::RemoveListener(FDelegateHandle Handle)
{
    Listener.Remove(Handle);
}
void UPriestMainMenuWidget::InvokeViewEvent(EViewEventType EventType, UEventParameterBase* Parameter)
{
    Listener.Broadcast(this, EventType, Parameter);
}
void UPriestMainMenuWidget::ShowStatusMessage(const FText& Message)
{
	if (MenuStatusText)
	{
		MenuStatusText->SetText(Message);
	}
}

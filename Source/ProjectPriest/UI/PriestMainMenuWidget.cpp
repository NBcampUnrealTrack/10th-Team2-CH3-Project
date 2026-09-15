#include "PriestMainMenuWidget.h"
#include "JUtility.h"
#include "Engine/Engine.h"
#include "PriestMainMenuPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "PriestMenuMvc.h"
#include "UObject/StrongObjectPtr.h"
#include "PriestInventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "PriestInventorySlotWidget.h"
#include "Components/UniformGridPanel.h"

bool UPriestMainMenuWidget::Initialize()
{
    if (bBindingsReady)
    {
        return true;
    }
    JASSERT_BOOL((Super::Initialize()), "%hs: Base widget initialization failed", __FUNCTION__);
    JASSERT_BOOL((IsValid(MenuSwitcher)), "%hs: Missing required MenuSwitcher binding", __FUNCTION__);
    JASSERT_BOOL((IsValid(LobbySwitcher)), "%hs: Missing required LobbySwitcher binding", __FUNCTION__);
    JASSERT_BOOL((MenuSwitcher->GetNumWidgets() == 3), "MenuSwitcher requires Title, Lobby and Credits pages");
    JASSERT_BOOL((LobbySwitcher->GetNumWidgets() == 2), "LobbySwitcher requires Region and Equipment pages");
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
	UGameInstance* Instance = GetGameInstance();
	JASSERT(IsValid(Instance), "%hs [%s]: GameInstance가 없습니다.", __FUNCTION__, *GetNameSafe(this));
	Inventory = Instance->GetSubsystem<UPriestInventorySubsystem>();
	JASSERT(IsValid(Inventory), "%hs [%s]: InventorySubsystem 연결에 실패했습니다.", __FUNCTION__, *GetNameSafe(this));
	Inventory->OnInventoryChanged.AddUniqueDynamic(this, &UPriestMainMenuWidget::RefreshInventory);
	RefreshInventory();
	ShowStatusMessage(FText::GetEmpty());
	RefreshPage();

}

void UPriestMainMenuWidget::NativeDestruct()
{
    if (Inventory)
    {
        Inventory->OnInventoryChanged.RemoveDynamic(this, &UPriestMainMenuWidget::RefreshInventory);
        Inventory = nullptr;
    }
    Super::NativeDestruct();
}

void UPriestMainMenuWidget::RefreshInventory()
{
    if (!Inventory)
    {
        return;
    }
    const TArray<FPriestOwnedItem> Items = Inventory->GetOwnedItems();
    if (InventorySummary)
    {
        InventorySummary->SetText(Items.IsEmpty()
            ? NSLOCTEXT("PriestInventory", "Empty", "보유 중인 아이템이 없습니다.")
            : FText::Format(NSLOCTEXT("PriestInventory", "Kinds", "총 {0}종"), FText::AsNumber(Items.Num())));
    }
    if (!InventoryGrid)
    {
        return;
    }
    InventoryGrid->ClearChildren();
    if (!InventorySlotClass || InventorySlotClass->HasAnyClassFlags(CLASS_Abstract))
    {
        UE_LOG(LogTemp, Warning, TEXT("Main menu: Set InventorySlotClass to WBP_InventorySlot in Class Defaults."));
        return;
    }
    const int32 Columns = FMath::Max(1, InventoryColumns);
    for (int32 Index = 0; Index < Items.Num(); ++Index)
    {
        UPriestInventorySlotWidget* SlotWidget = CreateWidget<UPriestInventorySlotWidget>(GetOwningPlayer(), InventorySlotClass);
        if (SlotWidget)
        {
            SlotWidget->SetItem(Items[Index]);
            InventoryGrid->AddChildToUniformGrid(SlotWidget, Index / Columns, Index % Columns);
        }
        else
        {
            JError("%hs: 슬롯 생성 실패. ItemId=%s, Class=%s", __FUNCTION__, *Items[Index].ItemId.ToString(), *GetNameSafe(InventorySlotClass.Get()));
        }
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
void UPriestMainMenuWidget::ShowCredits()
{
    SendRequest(EPriestMenuAction::Credits);
}
void UPriestMainMenuWidget::SwitchLobbyTab()
{
    SendRequest(EPriestMenuAction::SwitchTab);
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
bool UPriestMainMenuWidget::SendRequest(EPriestMenuAction Action)
{
    TStrongObjectPtr<UPriestMenuRequest> Request(NewObject<UPriestMenuRequest>());
    Request->Action = Action;
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

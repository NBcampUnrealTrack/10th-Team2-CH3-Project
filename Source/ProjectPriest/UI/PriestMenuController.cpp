#include "PriestMenuController.h"

#include "Engine/Engine.h"
#include "JUtility.h"
#include "PriestMainMenuPlayerController.h"
#include "PriestMainMenuWidget.h"
#include "PriestMenuModel.h"
#include "PriestMenuRequest.h"

void UPriestMenuController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
	UPriestMenuModel* Model = GetModel<UPriestMenuModel>();
	UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();
	JASSERT((IsValid(Model)), "Menu model is invalid. Connect the model before updating the menu.");
	JASSERT((IsValid(View)), "Menu view is invalid. Connect the view before updating the menu.");
	if (InModel != Model)
	{
		return;
	}
	View->ApplyMenuState(Model->GetPage(), Model->GetTab());
}

void UPriestMenuController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
	UPriestMenuModel* Model = GetModel<UPriestMenuModel>();
	UPriestMainMenuWidget* View = GetView<UPriestMainMenuWidget>();
	UPriestMenuRequest* Request = Cast<UPriestMenuRequest>(Parameter);
	JASSERT((IsValid(Model)), "Menu model is invalid.");
	JASSERT((IsValid(View)), "Menu view is invalid.");
	if (InView != View || EventType != EViewEventType::ButtonClicked)
	{
		return;
	}
	JASSERT((IsValid(Request)), "Menu button event requires a PriestMenuRequest parameter.");
	APriestMainMenuPlayerController* Owner = Cast<APriestMainMenuPlayerController>(View->GetOwningPlayer());
	JASSERT((IsValid(Owner)), "Menu view must be owned by PriestMainMenuPlayerController.");
	if (!Owner->CanProcessMenuRequest())
	{
		return;
	}
	switch (Request->Action)
	{
	case EPriestMenuAction::Title:
	{
		Model->SetState(EPriestMenuPage::Title, EPriestLobbyTab::Region);
		break;
	}

	case EPriestMenuAction::Regions:
	{
		Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Region);
		break;
	}

	case EPriestMenuAction::Equipment:
	{
		Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Equipment);
		break;
	}

	case EPriestMenuAction::Crafting:
	{
		Model->SetState(EPriestMenuPage::Lobby, EPriestLobbyTab::Crafting);
		break;
	}

	case EPriestMenuAction::Credits:
	{
		Model->SetState(EPriestMenuPage::Credits, Model->GetTab());
		break;
	}

	case EPriestMenuAction::SwitchTab:
	{
		const bool bCanSwitchTab = Model->GetPage() == EPriestMenuPage::Lobby && Request->TabDirection != 0;

		if (!bCanSwitchTab)
		{
			break;
		}

		constexpr int32 TabCount = 3;
		const int32 CurrentIndex = StaticCast<int32>(Model->GetTab());
		const int32 Direction = Request->TabDirection > 0 ? 1 : -1;
		const int32 NextIndex = (CurrentIndex + Direction + TabCount) % TabCount;
		Model->SetState(EPriestMenuPage::Lobby, StaticCast<EPriestLobbyTab>(NextIndex));
		break;
	}

	case EPriestMenuAction::StartStage:
	{
		const bool bIsLobbyPage = Model->GetPage() == EPriestMenuPage::Lobby;
		const bool bIsRegionTab = Model->GetTab() == EPriestLobbyTab::Region;

		const bool bCanStartStage = bIsLobbyPage && bIsRegionTab;

		if (!bCanStartStage)
		{
			break;
		}

		Owner->StartStageOne();
		break;
	}

	case EPriestMenuAction::Quit:
	{
		Owner->RequestQuitGame();
		break;
	}

	default:
	{
		break;
	}
	}
}

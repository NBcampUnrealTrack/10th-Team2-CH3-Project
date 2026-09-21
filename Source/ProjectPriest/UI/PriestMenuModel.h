#pragma once

#include "CoreMinimal.h"
#include "MvcModel.h"
#include "PriestMenuTypes.h"
#include "PriestMenuModel.generated.h"

UCLASS()
class PROJECTPRIEST_API UPriestMenuModel : public UObject, public IMvcModel
{
	GENERATED_BODY()

public:
	EPriestMenuPage GetPage() const
	{
		return Page;
	}

	EPriestLobbyTab GetTab() const
	{
		return Tab;
	}

	void SetState(EPriestMenuPage NewPage, EPriestLobbyTab NewTab);

	virtual FDelegateHandle AddListener(UMvcControl* Control) override;
	virtual void RemoveListener(FDelegateHandle Handle) override;
	virtual void InvokePropertyChanged(uint8 PropertyName) override;

private:
	EPriestMenuPage Page = EPriestMenuPage::Title;
	EPriestLobbyTab Tab = EPriestLobbyTab::Region;
	FModelChangedDelegate Changed;
};

#pragma once

#include "CoreMinimal.h"
#include "MvcEvents.h"
#include "PriestMenuTypes.h"
#include "PriestMenuRequest.generated.h"

UCLASS()
class PROJECTPRIEST_API UPriestMenuRequest : public UEventParameterBase
{
	GENERATED_BODY()

public:
	UPriestMenuRequest()
	{
		EventType = EViewEventType::ButtonClicked;
	}

	EPriestMenuAction Action = EPriestMenuAction::Title;
	int32 TabDirection = 0;
};

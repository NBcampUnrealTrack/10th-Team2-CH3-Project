#pragma once

#include "CoreMinimal.h"
#include "PriestMenuTypes.generated.h"

// Values must match the child order of WBP's MenuSwitcher.
UENUM(BlueprintType)
enum class EPriestMenuPage : uint8
{
	Title,
	Lobby,
	Credits
};

// Values must match the child order of WBP's LobbySwitcher.
UENUM(BlueprintType)
enum class EPriestLobbyTab : uint8
{
	Region,
	Equipment,
	Crafting
};

UENUM()
enum class EPriestMenuAction : uint8
{
	Title,
	Regions,
	Equipment,
	Crafting,
	Credits,
	SwitchTab,
	StartStage,
	Quit
};

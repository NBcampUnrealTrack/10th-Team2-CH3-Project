#include "PriestMainMenuGameMode.h"
#include "PriestMainMenuPlayerController.h"

APriestMainMenuGameMode::APriestMainMenuGameMode()
{
	PlayerControllerClass = APriestMainMenuPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

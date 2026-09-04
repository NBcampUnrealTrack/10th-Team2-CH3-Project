#include "TestPlayerController.h"
#include "EnhancedInputSubsystems.h"

ATestPlayerController::ATestPlayerController() 
	: InputMappingContext(nullptr),
	  FireAction(nullptr)
{
}

void ATestPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			if (InputMappingContext) {
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
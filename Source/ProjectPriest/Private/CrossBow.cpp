#include "CrossBow.h"
#include "IngameGameMode.h"
#include "JUtility.h"

// Sets default values
ACrossBow::ACrossBow()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACrossBow::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	AIngameGameMode* GameMode = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

	JASSERT(IsValid(GameMode), "GameMode is null");
	
	GameMode->OnCrossBowPickuped();
}
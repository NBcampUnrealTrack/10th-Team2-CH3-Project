#include "BossRoomOpenInteractor.h"
#include "Components/BoxComponent.h"
#include "IngameGameMode.h"
#include "IngameGameState.h"
#include "JUtility.h"

// Sets default values
ABossRoomOpenInteractor::ABossRoomOpenInteractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	
	SetRootComponent(SceneRoot);
	BoxCollision->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ABossRoomOpenInteractor::BeginPlay()
{
	Super::BeginPlay();
	
	AIngameGameState* IngameGameState 
		= Cast<AIngameGameState>(GetWorld()->GetGameState());
	
	JASSERT(IsValid(IngameGameState), "Gamestate is not IngameGameState or nullptr");
	
	IngameGameState->GetOnDoorVisibiliyChangedDelegate()
		.AddUObject(this, &ABossRoomOpenInteractor::OnDoorVisibilityChanged);
	
	OnDoorVisibilityChanged(IngameGameState->IsExitAvailable());
}

// Called every frame
void ABossRoomOpenInteractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossRoomOpenInteractor::OnDoorVisibilityChanged(bool Visibility)
{
	if (!Visibility)
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	BP_OnDoorVisibilityChanged(Visibility);
}
#include "Sevarog.h"
#include "Components/SkeletalMeshComponent.h"
#include "JUtility.h"
#include "SpawnActor.h"

// Sets default values
ASevarog::ASevarog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



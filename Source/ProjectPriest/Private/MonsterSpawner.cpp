#include "MonsterSpawner.h"
#include "Components/BoxComponent.h"
#include "JUtility.h"
#include "NavigationSystem.h"
#include "GameFramework/GameModeBase.h"
#include "IngameGameMode.h"
#include "EnemyCharacter.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

    SetRootComponent(SceneRoot);
    BoxComponent->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
    Super::BeginPlay();

    JASSERT(MonsterSpawnCount > 0, "Monster spawn count <= 0");

    UNavigationSystemV1* NavSystem
        = UNavigationSystemV1::GetCurrent(GetWorld());

    JASSERT(IsValid(NavSystem), "Nav systrem is invalid");

    FActorSpawnParameters SpawnParameter;
    SpawnParameter.SpawnCollisionHandlingOverride 
        = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int i = 0; i < MonsterSpawnCount; ++i)
    {
        //GetRandom Posistion from box

        const FVector Extent = BoxComponent->GetUnscaledBoxExtent();
        const FVector LocalRandomPoint(
            FMath::FRandRange(-Extent.X , Extent.X)
            , FMath::FRandRange(-Extent.Y, Extent.Y)
            , FMath::FRandRange(-Extent.Z, Extent.Z));

        FVector RandomPosition 
            = BoxComponent->GetComponentTransform().TransformPosition(LocalRandomPoint);

        FNavLocation NavigatableRandomPosition;
        if (!NavSystem->ProjectPointToNavigation(RandomPosition, NavigatableRandomPosition))
        {
            JError("Can not found random location : %f, %f, %f"
                , RandomPosition.X, RandomPosition.Y, RandomPosition.Z);

            continue;
        }

        int RandomIndex = FMath::RandRange(0, MonsterClasses.Num() - 1);
        TSubclassOf<AEnemyCharacter> ToSpawnClass = MonsterClasses[RandomIndex];
        
        AEnemyCharacter* SpawnedActor
            = GetWorld()->SpawnActor<AEnemyCharacter>(
                ToSpawnClass
                , NavigatableRandomPosition.Location
                , FRotator::ZeroRotator
                , SpawnParameter);

        AIngameGameMode* IngameGameMode
            = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

        JASSERT(IsValid(IngameGameMode), "Game mode is not IngameGameMode");
    }
}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
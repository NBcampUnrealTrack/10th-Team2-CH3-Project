#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

class AEnemyCharacter;
class UBoxComponent;

UCLASS()
class PROJECTPRIEST_API AMonsterSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMonsterSpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Monster Spawner ===|Components")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Monster Spawner ===|Components")
    TObjectPtr<UBoxComponent> BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Monster Spawner ===|Properties")
    TArray<TSubclassOf<AEnemyCharacter>> MonsterClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Monster Spawner ===|Properties")
    int MonsterSpawnCount;
};

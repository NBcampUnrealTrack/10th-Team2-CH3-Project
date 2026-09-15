// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UPrimitiveComponent;
struct FHitResult;

UCLASS()
class PROJECTPRIEST_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Components")
    TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Components")
    TObjectPtr<UStaticMeshComponent> StaticMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Components")
    TObjectPtr<UNiagaraComponent> Vfx;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Components")
    TObjectPtr<USphereComponent> Collision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Properties")
    float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Properties")
	float LifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Projectile ===|Properties")
	int Damage;
	
	float SpawnedTime;
	bool ReservedDestroying;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AoEObject.generated.h"

class UNiagaraComponent;

/*
* NOTICE: Collision callbacks are must be connected to collsion component in blueprint
*/
UCLASS()
class PROJECTPRIEST_API AAoEObject : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AAoEObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);


    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "==AoE==|Components")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "==AoE==|Components")
    TObjectPtr<UNiagaraComponent> Vfx;


    /////////////////////////////////////////////
    // PROPERTIES
    /////////////////////////////////////////////

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "==AoE==|Properties")
    float DelayFromStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "==AoE==|Properties")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "==AoE==|Properties")
    float DamageDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "==AoE==|Properties")
    float EachDamage;

    UPROPERTY()
    TObjectPtr<AActor> TargetActor;

    float StartTime;
    bool bIsStartDelay;
    float NextDamageTime;
};

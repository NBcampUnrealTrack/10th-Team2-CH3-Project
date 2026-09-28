// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossRoomOpenInteractor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTPRIEST_API ABossRoomOpenInteractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRoomOpenInteractor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void OnDoorVisibilityChanged(bool Visibility);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDoorVisibilityChanged(bool Visibility);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "===Boss Room Interactor===|Components")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "===Boss Room Interactor===|Components")
	TObjectPtr<UBoxComponent> BoxCollision;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAIController.h"
#include "SevarogAiController.generated.h"

UCLASS()
class PROJECTPRIEST_API ASevarogAiController : public AMonsterAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASevarogAiController();

	UFUNCTION()
	void OnHealthRateChanged(float HealthRate);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== ASevarogAiController ===")
	FName HealthRateKeyName;
};

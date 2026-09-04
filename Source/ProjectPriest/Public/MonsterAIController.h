// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPRIEST_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();

protected:
	virtual void BeginPlay() override;
	//폰을 소유할 때 호출되는 함수
	virtual void OnPossess(APawn* InPawn) override;

private:
	void MoveToRandomLocation();

	FTimerHandle RandomMoveTimer;

	// 몬스터가 이동할 반경
	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveRadius = 1000.0f;

};

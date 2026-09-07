// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
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

	//BT를 시작하는 함수
	void StartBehaviorTree();

protected:
	//BT 포인터
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;

private:

};

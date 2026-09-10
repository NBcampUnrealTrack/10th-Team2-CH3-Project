// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MonsterAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class PROJECTPRIEST_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	//BT를 시작하는 함수
	void StartBehaviorTree();

	AActor* GetDetectedPlayer();

	FVector GetDetectedPlayerLocation();


protected:
	//AI 감지 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	// 시야 감지 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	//감지된 액터
	UPROPERTY()
	AActor* DetectedPlayer = nullptr;

	//감지된 위치
	UPROPERTY()
	FVector DetectedPlayerLocation = FVector::ZeroVector;


	//BT 포인터
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;

private:

};

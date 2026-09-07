// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AMonsterAIController::AMonsterAIController()
{

}

void AMonsterAIController::StartBehaviorTree()
{
	if (BehaviorTree)//엔진(에디터)에서 지정을 하였는지 확인
	{
		RunBehaviorTree(BehaviorTree);
		//UE_LOG(LogTemp, Warning, TEXT("Behavior Tree start"));
	}
}
 
void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	StartBehaviorTree();





}	

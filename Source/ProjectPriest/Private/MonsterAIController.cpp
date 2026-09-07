// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"


AMonsterAIController::AMonsterAIController()
{

}

void AMonsterAIController::StartBehaviorTree()
{
	if (BehaviorTree)
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

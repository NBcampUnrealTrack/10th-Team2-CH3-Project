// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackRange.h"
#include "EnemyCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "JUtility.h"

UBTTask_AttackRange::UBTTask_AttackRange()
{
	NodeName = TEXT("AttackRange");
}

EBTNodeResult::Type UBTTask_AttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	FVector PlayerLocation = BlackboardComp->GetValueAsVector(TEXT("PlayerVector"));

	AAIController* Aicon = OwnerComp.GetAIOwner();
	if (!Aicon)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* Monster = Cast<AEnemyCharacter>(Aicon->GetPawn());
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Dist(
		PlayerLocation,
		Monster->GetActorLocation()
	);

	if (Monster->GetAttackRange() >= Distance)
	{
		BlackboardComp->SetValueAsBool(TEXT("AttackRange"),true);
		return EBTNodeResult::Succeeded;
	}

	BlackboardComp->SetValueAsBool(TEXT("AttackRange"),false);
	return EBTNodeResult::Succeeded;
}

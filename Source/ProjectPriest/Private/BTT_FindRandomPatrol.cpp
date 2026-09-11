// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomPatrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "NavigationSystem.h"



UBTT_FindRandomPatrol::UBTT_FindRandomPatrol()
{
	NodeName = "FindRandomPatrol";
}

EBTNodeResult::Type UBTT_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	AAIController* Aicom = OwnerComp.GetAIOwner();

	if (!Aicom)
		return EBTNodeResult::Failed;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Aicom->GetPawn());

	if (!EnemyCharacter)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSystem)
		return EBTNodeResult::Failed;

	FVector PlayerVector = BlackboardComp->GetValueAsVector(TEXT("PlayerVector"));

	int32 SearchCount = BlackboardComp->GetValueAsInt(TEXT("SearchCount"));

	FNavLocation RandomLocation;

	bool bFoundLocation = false;

	if(PlayerVector != FVector::ZeroVector && SearchCount > 0)
	{
		bFoundLocation = NavSystem->GetRandomReachablePointInRadius(
			PlayerVector,
			EnemyCharacter->GetPatrolRadius(),
			RandomLocation
		);
		if (bFoundLocation)
		{
			BlackboardComp->SetValueAsInt(TEXT("SearchCount"), SearchCount -1);
		}
	}

	else
	{
		bFoundLocation = NavSystem->GetRandomReachablePointInRadius(
			EnemyCharacter->GetPatrolOrigin(),
			EnemyCharacter->GetPatrolRadius(),
			RandomLocation
		);
	}


	if (bFoundLocation)
	{
		BlackboardComp->SetValueAsVector(TEXT("PatrolLocation"), RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

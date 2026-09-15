// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecoratorAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "AIController.h"

bool UBTDecoratorAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,	uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return false;
	}

	ACharacter* Player = Cast<ACharacter>(BlackboardComp->GetValueAsObject(TEXT("Player")));
	if (!Player)
	{
		return false;
	}

	AAIController* Aicon = OwnerComp.GetAIOwner();
	if (!Aicon)
	{
		return false;
	}

	AEnemyCharacter* Monster = Cast<AEnemyCharacter>(Aicon->GetPawn());
	if (!Monster)
	{
		return false;
	}

	float Distance = FVector::Dist(
		Player->GetActorLocation(),
		Monster->GetActorLocation()
	);

	if (Monster->GetAttackRange() >= Distance)
	{
		return true;
	}

	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CombatState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_CombatState::UBTService_CombatState()
{
	NodeName = "Update Combat State";
}

void UBTService_CombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Aicom = OwnerComp.GetAIOwner();

	APawn* AIPawn = Aicom->GetPawn();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!AIPawn || !playerPawn)
	{
		return;
	}

	//시야로 하고싶은데 흠
	//부체꼴? 콜리전을 만들어서 콜리전에 따라 bool값?
	float Distance = FVector::Distance(AIPawn->GetActorLocation(), playerPawn->GetActorLocation());

	//에디터 상 조절 할수있도록 불러오기? 몬스터? BB?
	float CombatDistance = 500;

	if (Distance <= CombatDistance)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), true);
	}
}
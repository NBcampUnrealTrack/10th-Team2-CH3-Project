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
		return;

	//시야를 만들기
	float Distance = FVector::Distance(//두 값의 차이
		AIPawn->GetActorLocation(),//몬스터의 위치
		playerPawn->GetActorLocation()//플레이어의 위치
	);

	//에디터 상 조절 할수있도록 몬스터 변수 지정해서 불러오기
	float CombatDistance = 500;

	//시야를 만들면 시야 + 거리? 아니면 시야 자체의 거리를 설정 할수있나?
	if (Distance <= CombatDistance)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), false);
	}
}
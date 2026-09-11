// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayer::UBTTask_FindPlayer()
{
	NodeName = TEXT("Find Player");
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//블랙보드 컴포넌트 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
		return EBTNodeResult::Failed;

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);//불러올 곳, 불러올 플레이어의 번호

	if (!playerPawn) 
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSystem)
		return EBTNodeResult::Failed;

	FNavLocation ProjectedLocation;

	bool bFoundNavigationLocation = NavSystem->ProjectPointToNavigation(
		playerPawn->GetActorLocation(),
		ProjectedLocation,
		FVector(200, 200, 500)
	);

	if (!bFoundNavigationLocation)
		return EBTNodeResult::Failed;

	BlackboardComp->SetValueAsVector(TEXT("PlayerVector"), ProjectedLocation.Location);

    //TODO REMOVE BY Joo jung yeol
    BlackboardComp->SetValueAsObject(TEXT("Player"), playerPawn);

	return EBTNodeResult::Succeeded;	
}
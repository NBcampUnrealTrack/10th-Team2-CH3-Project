// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CombatState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterAIController.h"
#include "GameFramework/Pawn.h"

UBTService_CombatState::UBTService_CombatState()
{
	NodeName = "Update Combat State";
}

void UBTService_CombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	AMonsterAIController* AIController = Cast<AMonsterAIController>(AICon);

	if (!AIController)
		return OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), false);

	if (playerPawn != AIController->GetDetectedPlayer())
		return OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), false);

	FVector DetectedPlayerLocation = AIController->GetDetectedPlayerLocation();
	if (FVector::ZeroVector == DetectedPlayerLocation)
		return OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), false);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsCombat"), true);
}
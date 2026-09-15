// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CombatState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterAIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "JUtility.h"

UBTService_CombatState::UBTService_CombatState()
{
	NodeName = "Update Combat State";
}

void UBTService_CombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	BlackboardComp -> SetValueAsObject(TEXT("Player"), Cast<ACharacter>(playerPawn));

	AMonsterAIController* AIController = Cast<AMonsterAIController>(AICon);

	if (!AIController)
	{
		return BlackboardComp->SetValueAsBool(TEXT("IsCombat"), false);
	}


	if (playerPawn != AIController->GetDetectedPlayer())
	{
		return BlackboardComp->SetValueAsBool(TEXT("IsCombat"), false);
	}

	//FVector DetectedPlayerLocation = AIController->GetDetectedPlayerLocation();

	//if (FVector::ZeroVector == DetectedPlayerLocation)
	//	return BlackboardComp->SetValueAsBool(TEXT("IsCombat"), false);

	//BlackboardComp->SetValueAsVector(TEXT("PlayerVector"), DetectedPlayerLocation);

	BlackboardComp->SetValueAsInt(TEXT("SearchCount"), 3);

	BlackboardComp->SetValueAsBool(TEXT("IsCombat"), true);
}
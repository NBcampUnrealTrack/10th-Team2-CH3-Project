#include "BTTask_Die.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "JUtility.h"

EBTNodeResult::Type UBTTask_Die::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AiController = OwnerComp.GetAIOwner();
	APawn* Pawn = AiController->GetPawn();
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Pawn);
	JASSERT_TASK(IsValid(EnemyCharacter), "Enemy is invalid");
	
	EnemyCharacter->Die();
	
	return EBTNodeResult::Succeeded;
}

#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAIController.h"
#include "Engine/DamageEvents.h"
#include "JUtility.h"

UBTTask_Attack::UBTTask_Attack()
{
    bCreateNodeInstance = true;

    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AMonsterAIController* MonsterController 
        = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

    JASSERT_RETURN(IsValid(MonsterController)
        , EBTNodeResult::Failed
        , "It is not MonsterController");

    AEnemyCharacter* EnemyCharacter
        = Cast<AEnemyCharacter>(MonsterController->GetPawn());

    JASSERT_RETURN(IsValid(EnemyCharacter)
        , EBTNodeResult::Failed
        , "It is not MonsterController");

    EnemyCharacter->Attack(nullptr);

    return EBTNodeResult::Succeeded;
}
#include "BTTask_SetAttackTarget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAIController.h"
#include "EnemyCharacter.h"
#include "JUtility.h"

UBTTask_SetAttackTarget::UBTTask_SetAttackTarget()
{
    NodeName = TEXT("Set Attack Target");
}

EBTNodeResult::Type UBTTask_SetAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AMonsterAIController* MonsterAiCntroller 
        = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

    JASSERT_RETURN(IsValid(MonsterAiCntroller), EBTNodeResult::Failed, "AiOwner is not AMonsterAiController");

    AEnemyCharacter* EnemyCharacter 
        = Cast<AEnemyCharacter>(MonsterAiCntroller->GetPawn());

    JASSERT_RETURN(IsValid(EnemyCharacter), EBTNodeResult::Failed, "Pawn is not AEnemyCharacter");

    //read blackboard
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    ACharacter* TargetCharacter 
        = Cast<ACharacter>(BlackboardComponent->GetValueAsObject(TargetValueName));
    
    JASSERT_RETURN(IsValid(TargetCharacter), EBTNodeResult::Failed, "Can not get target from blackboard");
    
    EnemyCharacter->SetAttackTarget(TargetCharacter);

    return EBTNodeResult::Succeeded;
}
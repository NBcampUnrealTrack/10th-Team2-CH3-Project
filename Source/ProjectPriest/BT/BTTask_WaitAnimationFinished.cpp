#include "BTTask_WaitAnimationFinished.h"
#include "MonsterAIController.h"
#include "EnemyCharacter.h"
#include "JUtility.h"

UBTTask_WaitAnimationFinished::UBTTask_WaitAnimationFinished()
{
    NodeName = TEXT("Wait Animation Finished");
    bNotifyTick = true;
}


EBTNodeResult::Type UBTTask_WaitAnimationFinished::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    return EBTNodeResult::InProgress;
}

void UBTTask_WaitAnimationFinished::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AMonsterAIController* MonsterController
        = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

    JASSERT(IsValid(MonsterController) , "Not Monster Controller");

    AEnemyCharacter* EnemyCharacter
        = Cast<AEnemyCharacter>(MonsterController->GetPawn());
    JASSERT(IsValid(EnemyCharacter) , "Not EnemyCharacter");

    EAttackAnimationState AnimationState
        = EnemyCharacter->GetAttackAnimationeState();

    switch (AnimationState)
    {
    case EAttackAnimationState::Error:
    case EAttackAnimationState::Interrupted:
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        break;

    case EAttackAnimationState::InProgress:
        FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
        break;

    case EAttackAnimationState::Finished:
        //JLog("Animation Finished");
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        break;
    }
}
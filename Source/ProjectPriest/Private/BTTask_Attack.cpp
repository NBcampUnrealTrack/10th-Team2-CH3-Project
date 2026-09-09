#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAIController.h"
#include "Engine/DamageEvents.h"

UBTTask_Attack::UBTTask_Attack()
{
    bCreateNodeInstance = true;

    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    CachedOwnerComponent = &OwnerComp;
    
    AEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromOwnerComp(OwnerComp);

    if (!bStartFlag)
    {
        bStartFlag = true;

        FApplyAttackDelegte Delegate;
        Delegate.BindUObject(this, &UBTTask_Attack::OnApplyAttack);

        EnemyCharacter->Attack(nullptr);

        return EBTNodeResult::InProgress;
    }
    else
    {
        switch (EnemyCharacter->GetAttackMontageState())
        {
        case EAttackMontageState::Error:
            return EBTNodeResult::Failed;

        case EAttackMontageState::InProgress:
            return EBTNodeResult::InProgress;

        case EAttackMontageState::Interrupted:
            return EBTNodeResult::Aborted;

        case EAttackMontageState::Finished:
            return EBTNodeResult::Succeeded;

        default:
            //error
            break;
        }
    }
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    bStartFlag = false;
    return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    bStartFlag = false;
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_Attack::OnApplyAttack()
{
    UBlackboardComponent* Blackboard = CachedOwnerComponent->GetBlackboardComponent();
    if (!Blackboard)
    {
        return;
    }

    UObject* TargetObject = Blackboard->GetValueAsObject(TargetValueName);
    ACharacter* TargetCharacter = Cast<ACharacter>(TargetObject);

    AEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromOwnerComp(*CachedOwnerComponent);
    
    FDamageEvent DamageEvent;
    TargetCharacter->TakeDamage(EnemyCharacter->GetDamage()
        , DamageEvent
        , CachedOwnerComponent->GetAIOwner()
        , EnemyCharacter);
}

AEnemyCharacter* UBTTask_Attack::GetEnemyCharacterFromOwnerComp(UBehaviorTreeComponent& OwnerComp)
{
    AAIController* AiController = OwnerComp.GetAIOwner();
    if (!AiController)
    {
        return nullptr;
    }

    ACharacter* AiCharacter = AiController->GetCharacter();
    if (!AiCharacter)
    {
        return nullptr;
    }

    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AiCharacter);
    if (!EnemyCharacter)
    {
        return nullptr;
    }

    return EnemyCharacter;
}

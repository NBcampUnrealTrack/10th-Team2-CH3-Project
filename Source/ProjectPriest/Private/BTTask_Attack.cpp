#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
    bCreateNodeInstance = true;

    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AiController = OwnerComp.GetAIOwner();
    if (!AiController)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* AiCharacter = AiController->GetCharacter();
    if (!AiCharacter)
    {
        return EBTNodeResult::Failed;
    }

    UAnimInstance* AiAnimInstance = AiCharacter->GetMesh()->GetAnimInstance();
    if (!AiAnimInstance)
    {
        return EBTNodeResult::Failed;
    }

    if (!MontageToPlaying)
    {
        return EBTNodeResult::Failed;
    }

    CachedOwnerComponent = &OwnerComp;
    CachedAnimInstance = AiAnimInstance;

    float MontagePlayResult = CachedAnimInstance->Montage_Play(MontageToPlaying, 1.0f);
    if (FMath::IsNearlyZero(MontagePlayResult))
    {
        return EBTNodeResult::Failed;
    }

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &UBTTask_Attack::OnMontageEnded);

    CachedAnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlaying);

    return EBTNodeResult::InProgress;
}

void UBTTask_Attack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!CachedAnimInstance)
    {
        return;
    }

    AEnemyCharacter* MyCharacter = Cast<AEnemyCharacter>(CachedOwnerComponent->GetOwner());
    UBlackboardComponent* Blackboard = CachedOwnerComponent->GetBlackboardComponent();
    UObject* TempTarget = Blackboard->GetValueAsObject(TargetValueName);
    ACharacter* EnemyCharacter = Cast<ACharacter>(TempTarget);
    MyCharacter->Attack(EnemyCharacter);

    //TODO: 이곳에 데미지 주는 것을 구현하기
    FinishLatentTask(*CachedOwnerComponent,
        bInterrupted ?
        EBTNodeResult::Failed : 
        EBTNodeResult::Succeeded
    );
}
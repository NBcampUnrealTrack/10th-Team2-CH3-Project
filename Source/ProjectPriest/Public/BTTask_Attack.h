#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

class UAnimInstance;
class UBehaviorTreeComponent;
class AEnemyCharacter;

UCLASS()
class PROJECTPRIEST_API UBTTask_Attack : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
	UBTTask_Attack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    void OnApplyAttack();
    AEnemyCharacter* GetEnemyCharacterFromOwnerComp(UBehaviorTreeComponent& OwnerComp);
protected:
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Attacks==")
    FName TargetValueName;    

    TObjectPtr<UBehaviorTreeComponent> CachedOwnerComponent;

    bool bStartFlag;
};

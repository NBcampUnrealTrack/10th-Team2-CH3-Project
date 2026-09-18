#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetAttackTarget.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_SetAttackTarget: public UBTTask_BlackboardBase
{
    GENERATED_BODY()
public:
	UBTTask_SetAttackTarget();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== PROPERTIES ===")
    FName TargetValueName;
};

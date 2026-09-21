#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_WaitAnimationFinished.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_WaitAnimationFinished : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
	UBTTask_WaitAnimationFinished();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

protected:

};

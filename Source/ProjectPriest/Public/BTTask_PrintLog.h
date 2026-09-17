#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PrintLog.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_PrintLog : public UBTTask_BlackboardBase
{
    GENERATED_BODY()
public:
	UBTTask_PrintLog();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    FString Message;
};

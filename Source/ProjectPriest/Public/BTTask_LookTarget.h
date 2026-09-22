#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LookTarget.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_LookTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_LookTarget();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

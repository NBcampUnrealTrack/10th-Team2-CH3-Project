#pragma once

#include "CoreMinimal.h"
#include <BehaviorTree/BTTaskNode.h>
#include "BTTask_Die.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_Die : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

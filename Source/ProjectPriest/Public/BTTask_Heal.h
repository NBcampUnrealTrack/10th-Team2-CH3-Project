#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Heal.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_Heal : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Heal();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="=== HEAL ===")
	float HealSpeed;
};

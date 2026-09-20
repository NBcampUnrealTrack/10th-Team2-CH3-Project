#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IncreasePhase.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTTask_IncreasePhase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_IncreasePhase();
	
	virtual FString GetStaticDescription() const override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="=== IncreasePhase ===")
	int Amount;
};

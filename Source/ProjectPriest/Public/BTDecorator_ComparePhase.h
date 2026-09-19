#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ECompareOperatorType.h"
#include "BTDecorator_ComparePhase.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTDecorator_ComparePhase : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ComparePhase();
	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory) const override;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="=== IncreasePhase ===")
	ECompareOperatorType CompareOperator;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="=== IncreasePhase ===")
	int ExpectValue;
};

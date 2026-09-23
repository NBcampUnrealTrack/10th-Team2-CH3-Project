#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "ECompareOperatorType.h"
#include "BTD_CompareHealthRate.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTD_CompareHealthRate : public UBTDecorator
{
	GENERATED_BODY()
	
public:	
	UBTD_CompareHealthRate();
	virtual FString GetStaticDescription() const override;
	
protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool EvaluateCondition(UBehaviorTreeComponent& OwnerComp) const;
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "=== Compare Health Rate ===")
	ECompareOperatorType CompareType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "=== Compare Health Rate ===")
	float Rate;
};

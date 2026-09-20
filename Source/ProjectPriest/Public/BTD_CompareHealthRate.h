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
	virtual FString GetStaticDescription() const override;
	
protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "=== Compare Health Rate ===")
	ECompareOperatorType CompareType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "=== Compare Health Rate ===")
	float Rate;
};

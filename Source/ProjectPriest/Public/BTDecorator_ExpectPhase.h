#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ExpectPhase.generated.h"

UCLASS()
class PROJECTPRIEST_API UBTDecorator_ExpectPhase : public UBTDecorator 
{
	GENERATED_BODY()
public:
	UBTDecorator_ExpectPhase();
	
protected:
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "=== ExpectPhase ===")
	int ExpectedPhase;
};
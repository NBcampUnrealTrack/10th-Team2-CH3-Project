#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

class UAnimInstance;
class UBehaviorTreeComponent;

UCLASS()
class PROJECTPRIEST_API UBTTask_Attack : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
	UBTTask_Attack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UFUNCTION(BlueprintCallable)
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Attack==")
    TObjectPtr<UAnimMontage> MontageToPlaying;

    TObjectPtr<UAnimInstance> CachedAnimInstance;
    TObjectPtr<UBehaviorTreeComponent> CachedOwnerComponent;


};

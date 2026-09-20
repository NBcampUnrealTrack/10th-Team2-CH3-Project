#include "BTTask_Heal.h"

#include "AIController.h"
#include "JUtility.h"
#include "Sevarog.h"

UBTTask_Heal::UBTTask_Heal()
{
	NodeName = TEXT("Heal");
	
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Heal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	JASSERT(HealSpeed >= 0, "Heal Speed must be greater or equal than 0.");
	
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	APawn* Pawn = OwnerController->GetPawn();
	ASevarog* Sevarog = Cast<ASevarog>(Pawn);
	
	float HealAmount = HealSpeed * DeltaSeconds;
	
	Sevarog->Heal(HealAmount);
	
	if(Sevarog->GetHealthRate() >= 1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

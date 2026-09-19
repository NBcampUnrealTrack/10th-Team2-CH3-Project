#include "BTTask_IncreasePhase.h"
#include "Sevarog.h"
#include "AIController.h"
#include "JUtility.h"

UBTTask_IncreasePhase::UBTTask_IncreasePhase()
{
	NodeName = FString("IncreasePhase");
}

FString UBTTask_IncreasePhase::GetStaticDescription() const
{
	Super::GetStaticDescription();
	
	FString Description = FString::Printf(TEXT("Phase += %d"), Amount);
	
	return Description;
}

EBTNodeResult::Type UBTTask_IncreasePhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//TODO: 아래의 코드가 중복되고 있음 어디에 묶어서 사용 요망
	AAIController* AiController = OwnerComp.GetAIOwner();
    JASSERT_TASK(IsValid(AiController), "AiController cannot be null");
	
	
	APawn* Pawn = AiController->GetPawn();
    JASSERT_TASK(Pawn, "Pawn cannot be null");
			 
	ASevarog* Sevarog = Cast<ASevarog>( Pawn);
	JASSERT_TASK(IsValid(Sevarog), "Severerty cannot be null");
	
    Sevarog->IncreasePhase();

    return EBTNodeResult::Succeeded;
}

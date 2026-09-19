#include "BTDecorator_ExpectPhase.h"
#include "AIController.h"
#include "JUtility.h" 
#include "Sevarog.h"

UBTDecorator_ExpectPhase::UBTDecorator_ExpectPhase()
{
	NodeName = TEXT("ExpectPhase");
}

bool UBTDecorator_ExpectPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	AAIController* AiController = OwnerComp.GetAIOwner();
	JASSERT_BOOL(IsValid(AiController), "%hs AiController cannot be null", __FUNCTION__);
	
	APawn* Pawn = AiController->GetPawn();
	JASSERT_BOOL(Pawn, "%hs Pawn cannot be null", __FUNCTION__);
	
	ASevarog* Sevarog = Cast<ASevarog>( Pawn);
	JASSERT_BOOL(IsValid(Sevarog), "%hs Severerty cannot be null", __FUNCTION__);
	
	return Sevarog->GetPhase() == ExpectedPhase;	
}

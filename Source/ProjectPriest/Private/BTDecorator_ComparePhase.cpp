#include "BTDecorator_ComparePhase.h"
#include "AIController.h"
#include "EditorMetadataOverrides.h"
#include "JUtility.h"
#include "Sevarog.h"
#include "GameFramework/Pawn.h"

UBTDecorator_ComparePhase::UBTDecorator_ComparePhase()
{
	NodeName = TEXT("ComparePhase");
}

FString UBTDecorator_ComparePhase::GetStaticDescription() const
{
	Super::GetStaticDescription();
	
	return FString::Printf(TEXT("Phase %s %d")\
		, *GET_ENUM_DISPLAY_STRING(ECompareOperatorType, CompareOperator)
		, ExpectValue
	);
}

bool UBTDecorator_ComparePhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	JASSERT_BOOL(IsValid(AIController), "AIController cannot be null");
	
	APawn* Pawn = AIController->GetPawn();
	JASSERT_BOOL(IsValid(Pawn), "Pawn cannot be null");
	
	ASevarog* Sevarog = Cast<ASevarog>(Pawn);
	JASSERT_BOOL(IsValid(Sevarog), "Sevarog cannot be null");
	
	int CurrentPhase = Sevarog->GetPhase();
	switch (CompareOperator)
	{
	case ECompareOperatorType::Equal:			return CurrentPhase == ExpectValue; 		
	case ECompareOperatorType::NotEqual:		return CurrentPhase != ExpectValue;		
	case ECompareOperatorType::Greater:			return CurrentPhase > ExpectValue;		
	case ECompareOperatorType::GreaterOrEqual:	return CurrentPhase >= ExpectValue;				
	case ECompareOperatorType::Less:			return CurrentPhase < ExpectValue;		
	case ECompareOperatorType::LessOrEqual:		return CurrentPhase <= ExpectValue;
	default:
		JError("%d Not supported yet", (int)CompareOperator);
		return false;
	}
}

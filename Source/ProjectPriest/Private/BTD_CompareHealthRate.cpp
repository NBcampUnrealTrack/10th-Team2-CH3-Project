#include "BTD_CompareHealthRate.h"

#include "AIController.h"
#include "JUtility.h"
#include "Sevarog.h"

bool UBTD_CompareHealthRate::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	JASSERT_RETURN(IsValid(OwnerController)
		, EBTNodeResult::Type::Failed
		, "OwnerController is invalid"
		);
	
	APawn* Pawn = OwnerController->GetPawn();
	JASSERT_RETURN(IsValid(Pawn)
		, EBTNodeResult::Type::Failed
		, "Pawn is invalid" 
		);
	
	ASevarog* EnemyCharacter = Cast<ASevarog>(Pawn);
	JASSERT_RETURN(IsValid(EnemyCharacter)
		, EBTNodeResult::Type::Failed
		, "Pawn is not Sevarog"
		);
	
	float HealthRate = EnemyCharacter->GetHealthRate();
	
	switch (CompareType)
	{
	case ECompareOperatorType::Equal:				return HealthRate == Rate;		
	case ECompareOperatorType::NotEqual:			return HealthRate != Rate;		
	case ECompareOperatorType::Greater:				return HealthRate > Rate;		
	case ECompareOperatorType::GreaterOrEqual:		return HealthRate >= Rate;		
	case ECompareOperatorType::Less:				return HealthRate < Rate;		
	case ECompareOperatorType::LessOrEqual:			return HealthRate <= Rate;		
	default:
		JError("%s is not supported yet"
			, *GET_ENUM_NAME_STRING(ECompareOperatorType, CompareType)
		);
		
		return false;
	}
}

FString UBTD_CompareHealthRate::GetStaticDescription() const
{
	Super::GetStaticDescription();

	return FString::Format(TEXT("Health rate {0} {1}")
		,{ 
			GET_ENUM_DISPLAY_STRING(ECompareOperatorType, CompareType)
			, Rate
			});
}

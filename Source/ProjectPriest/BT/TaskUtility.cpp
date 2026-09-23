#include "TaskUtility.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "JUtility.h"
#include "EnemyCharacter.h"

AEnemyCharacter* TaskUtility::GetEnemyCharacter(UBehaviorTreeComponent& OwnerComp)
{
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetOwner());
	JASSERT_NULLPTR(IsValid(Character), "Character cannot be null");

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Character);
	JASSERT_NULLPTR(IsValid(EnemyCharacter), "EnemyCharacter cannot be null");
	
	return EnemyCharacter;
}

#pragma once

class ACharacter;
class UBehaviorTreeComponent;
class AEnemyCharacter;

class TaskUtility
{
public:
	static AEnemyCharacter* GetEnemyCharacter(UBehaviorTreeComponent& OwnerComp);
	
};

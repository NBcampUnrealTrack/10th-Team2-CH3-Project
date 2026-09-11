// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CombatState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPRIEST_API UBTService_CombatState : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTService_CombatState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

#include "BTTask_LookTarget.h"
#include "AIController.h"

#include <BehaviorTree/BlackboardComponent.h>

#include "JUtility.h"

UBTTask_LookTarget::UBTTask_LookTarget()
{
	BlackboardKey.AddObjectFilter(
		this
		,GET_MEMBER_NAME_CHECKED(UBTTask_LookTarget, BlackboardKey)
		, UObject::StaticClass()
	);
}

EBTNodeResult::Type UBTTask_LookTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	JASSERT_TASK(BlackboardKey.IsSet(), "key is not setted");
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	UObject* FindObject = Blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName);
	JASSERT_TASK(IsValid(FindObject), "key is not setted");
	
	APawn* FindTarget = Cast<APawn>(FindObject);
	
	FVector FindTargetLocation = FindTarget->GetActorLocation();
	
	AAIController* AiController = OwnerComp.GetAIOwner();
    JASSERT_TASK(IsValid(AiController), "it is not ai controller");

    APawn* AiPawn = AiController->GetPawn();
    JASSERT_TASK(IsValid(AiPawn), "Ai pawn is invalid");

    FVector AiLocation = AiPawn->GetActorLocation();
	
	FVector PlanarTargetLocation = FVector::VectorPlaneProject(FindTargetLocation, FindTarget->GetActorUpVector());
	FVector PlanarOwnerLocation =  FVector::VectorPlaneProject(AiLocation, AiPawn->GetActorUpVector());
	FVector PlanarDirection = PlanarTargetLocation - PlanarOwnerLocation;
	
    AiPawn->SetActorRotation(PlanarDirection.Rotation());

	return EBTNodeResult::Succeeded;
}

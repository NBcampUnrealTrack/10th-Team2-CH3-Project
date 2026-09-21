#include "SpawnActor.h"
#include "JUtility.h"

void USpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    
	UWorld* World = MeshComp->GetWorld();
	
	//실제로 PREIVEW가 아닌 게임플레이에서만 동작하도록 한다.
	if (!World->IsGameWorld())
	{
		return;
	}
	
	APawn* Owner = Cast<APawn>(MeshComp->GetOwner());
	AController* OwnerController = MeshComp->GetOwner()->GetInstigatorController();
	FVector Location = MeshComp->GetOwner()->GetActorLocation();
	FRotator Rotation = FRotator::ZeroRotator;
    		
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = OwnerController;
    SpawnParams.Instigator = Owner;
    World->SpawnActor<AActor>( ActorToSpawn
        , Location
        , Rotation
        , SpawnParams);
}

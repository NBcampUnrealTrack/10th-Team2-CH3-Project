#pragma once

#include "CoreMinimal.h"
#include "SpawnActor.generated.h"

UCLASS()
class PROJECTPRIEST_API USpawnActor : public UAnimNotify
{
	GENERATED_BODY()
	
public:
    virtual void Notify(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        const FAnimNotifyEventReference& EventReference
    ) override;

    // Animation Editor에서 Notify를 선택했을 때 설정 가능
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===SpawnActor===")
    TSubclassOf<AActor> ActorToSpawn;
};

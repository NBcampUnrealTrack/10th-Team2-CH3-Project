#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

class UBoxComponent;
class APlayerCharacter;

UCLASS()
class PROJECTPRIEST_API AMeleeEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Collision")
    UBoxComponent* AttackCollision;

	//몬스터 공격
	virtual void Attack(ACharacter* PlayerCharacter) override;

    UFUNCTION(BlueprintCallable)
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    virtual void OnNotifyApplyDamage()override;

    const FName& GetRandomSessionName();

    UFUNCTION()
    void OnAttackRangeBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnAttackRangeEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

    TObjectPtr<APlayerCharacter> Player = nullptr;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    TObjectPtr<UAnimMontage> MontageToPlaying;

    TArray<FName> SessionNames;

    FName RandomSessionName;
};
 
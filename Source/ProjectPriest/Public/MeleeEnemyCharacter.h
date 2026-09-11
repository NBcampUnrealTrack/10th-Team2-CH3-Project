#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"


UCLASS()
class PROJECTPRIEST_API AMeleeEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Collision")
    USphereComponent* LeftHandCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Collision")
    USphereComponent* RightHandCollision;

	//몬스터 공격
	virtual void Attack(ACharacter* PlayerCharacter) override;

    UFUNCTION(BlueprintCallable)
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    virtual void OnNotifyApplyDamage()override;

    const FName& GetRandomSessionName();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    TObjectPtr<UAnimMontage> MontageToPlaying;

    TArray<FName> SessionNames;

    FName RandomSessionName;
};
 
#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "RangedEnemyCharacter.generated.h"

class AProjectile;
struct FTimerHandle;

UCLASS()
class PROJECTPRIEST_API ARangedEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ARangedEnemyCharacter();

protected:
	//몬스터 공격
	virtual void Attack(ACharacter* PlayerCharacter) override;
    virtual void SpawnBullet() const;
    void OnAttackDelayTimer();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Ranged Enemy ===|Properties")
    TSubclassOf<AProjectile> Bullet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Ranged Enemy ===|Properties")
    float AttackDelay;

    FTimerHandle AttackDelayTimerHandler;

};

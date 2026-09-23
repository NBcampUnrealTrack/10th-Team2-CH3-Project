#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "RangedEnemyCharacter.generated.h"

class AProjectile;
class UParticleSystem;
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

    virtual void Die()override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
    TObjectPtr<UParticleSystem> DeathParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Ranged Enemy ===|Properties")
    TSubclassOf<AProjectile> Bullet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Ranged Enemy ===|Properties")
    float AttackDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
    float ParticleTime;

    UPROPERTY()
    FTimerHandle DestroyParticle;

    FTimerHandle AttackDelayTimerHandler;

    UFUNCTION(BlueprintImplementableEvent, Category = "Death")
    void OnDeath();//블루 프린트에서 구현
};

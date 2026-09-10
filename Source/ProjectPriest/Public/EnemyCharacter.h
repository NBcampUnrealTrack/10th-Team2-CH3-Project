#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class USphereComponent;

UENUM(Blueprinttype)
enum class EAttackAnimationState : uint8
{
    InProgress      UMETA(DisplayName = "InProgress"),
    Finished        UMETA(DisplayName = "Finished"),
    Interrupted     UMETA(DisplayName = "Abort"),
    Error           UMETA(DisplayName = "Error")
};

DECLARE_DELEGATE(FApplyAttackDelegte);

//TODO: 몬스터 스탯을 테이블로 해야한다
UCLASS()
class PROJECTPRIEST_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	//몬스터의 기준위치
	UFUNCTION(BlueprintCallable)
	FVector GetPatrolOrigin();

	//몬스터의 순찰반경
	UFUNCTION(BlueprintCallable)
	float GetPatrolRadius();

    //사거리
	UFUNCTION(BlueprintCallable)
    float GetAttackRange();

    UFUNCTION(BlueprintCallable)
    virtual void OnNotifyApplyDamage();

    //몬스터 공격
    UFUNCTION(BlueprintCallable)
    virtual void Attack(ACharacter* PlayerCharacter);

    //몬스터 피격
    UFUNCTION(BlueprintCallable)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    //몬스터 사망
    UFUNCTION(BlueprintCallable)
    virtual void Die();

    /// <summary>
    /// Interface for Animation blueprint
    /// </summary>
    UFUNCTION(BlueprintPure)
    virtual bool GetIsDead();

    UFUNCTION(BlueprintPure)
    virtual bool GetShouldAttack();

    UFUNCTION(BlueprintPure)
    virtual bool HitThisFrame();
  
    float GetDamage();

    void SetAttackTarget(ACharacter* Target);

    EAttackAnimationState GetAttackAnimationeState();

protected:
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:

	//몬스터의 기준위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Patrol")
	FVector PatrolOrigin;

	//몬스터의 순찰반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolRadius;

	//몬스터 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Health;

	//몬스터 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Damage;

	//몬스터 최소 피해량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MinimumDamage;

    //몬스터 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AttackRange;

    //몬스터 방어력
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    bool bHitThisFrame;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    bool bIsDead;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    bool bShouldAttack;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    TObjectPtr<UAnimInstance> AnimInstance;

    TObjectPtr<ACharacter> AttackTarget;

    EAttackAnimationState AttackAnimationeState;
};

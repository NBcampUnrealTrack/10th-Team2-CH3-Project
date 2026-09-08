// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class USphereComponent;

//UENUM(BlueprintType)
//enum class EAnimationFinishedReason : uint8
//{
//    Finshed     UMETA(DisplayName = "Finished"),
//    Abort       UMETA(DisplayName = "Abort")
//    //TODO: 뭐... 나중에 더 추가할수도?
//};

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
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void PlayAttackAnimation();
    virtual void OnApplyDamage(int SequenceNumber);
    virtual void OnAttackAnimationFinished(const int Reson);

    //몬스터 공격
    UFUNCTION(BlueprintCallable)
    virtual void Attack(ACharacter* PlayerCharacter);

    //몬스터 피격
    UFUNCTION(BlueprintCallable)
    virtual void TakeDamage(float DamageAmount);

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
protected:

	////스피어 컴퍼넌트
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//USphereComponent* SphereComponent;

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

    //몬스터 방어력
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    bool bHitThisFrame;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    bool bIsDead;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    bool bShouldAttack;
};

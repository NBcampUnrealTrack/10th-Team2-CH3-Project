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

UENUM(Blueprinttype)
enum class EAttackMontageState : uint8
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

    UFUNCTION(BlueprintCallable)
    virtual void OnNotifyApplyDamage();

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

    UFUNCTION(BlueprintCallable)
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    EAttackMontageState GetAttackMontageState() const;
    float GetDamage();

    void SetApplyAttackDelegate(FApplyAttackDelegte& Delegate);
    void UnbindApplyAttackDelegate();

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
    TObjectPtr<UAnimMontage> MontageToPlaying;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For anim blueprint")
    TObjectPtr<UAnimInstance> AnimInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Enemy Character ===|For BT")
    FName TargetValueName;    

    EAttackMontageState AttackMontageState;
    FApplyAttackDelegte ApplyAttackDelegate;

    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

};

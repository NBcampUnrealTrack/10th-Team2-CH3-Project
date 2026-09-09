// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
//추후 플레이어 캐릭터 인클루드  
//#include "PlayerCharacter.h"
#include "MonsterAIController.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//AI 컨트롤러 class 설정
	AIControllerClass = AMonsterAIController::StaticClass();
	//생성시 AI Possess 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//스탯 초기화
	Health = 100.0f;
	Damage = 10.0f;
	Defense = 5.0f;
	MinimumDamage = 1.0f;//몬스터가 받는 최소피해

	PatrolRadius = 1000.0f;
}


void AEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
    float MontagePlayResult = AnimInstance->Montage_Play(MontageToPlaying, 1.0f);
    if (FMath::IsNearlyZero(MontagePlayResult))
    {
        //TODO: Error what to do
        AttackMontageState = EAttackMontageState::Error;
        return;
    }

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AEnemyCharacter::OnMontageEnded);

    AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlaying);
    AttackMontageState = EAttackMontageState::InProgress;
}

void AEnemyCharacter::TakeDamage(float DamageAmount)
{
	float ActualDamage = FMath::Max(DamageAmount - Defense, MinimumDamage);
	Health -= ActualDamage;
	if (Health <= 0)
	{
		Die();
	}
}

void AEnemyCharacter::Die()
{
	Destroy();
}
void AEnemyCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        AttackMontageState = EAttackMontageState::Interrupted;
    }
    else
    {
        AttackMontageState = EAttackMontageState::Finished;
    }
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PatrolOrigin = this->GetActorLocation();

    AnimInstance = GetMesh()->GetAnimInstance();
}

void AEnemyCharacter::OnNotifyApplyDamage()
{
    if (!IsValid(GEngine))
        return;
}

bool AEnemyCharacter::GetIsDead()
{
    return bIsDead;
}

bool AEnemyCharacter::GetShouldAttack()
{
    return bShouldAttack;
}

bool AEnemyCharacter::HitThisFrame()
{ 
    return bHitThisFrame;
}

EAttackMontageState AEnemyCharacter::GetAttackMontageState() const
{
    return AttackMontageState;
}

float AEnemyCharacter::GetDamage()
{
    return Damage;
}

void AEnemyCharacter::SetApplyAttackDelegate(FApplyAttackDelegte& Delegate)
{
    ApplyAttackDelegate = Delegate;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AEnemyCharacter::GetPatrolOrigin()
{
	return PatrolOrigin;
}

float AEnemyCharacter::GetPatrolRadius()
{
	return PatrolRadius;
}
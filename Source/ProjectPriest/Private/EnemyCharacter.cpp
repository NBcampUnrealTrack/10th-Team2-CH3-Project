// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
//추후 플레이어 캐릭터 인클루드  
//#include "PlayerCharacter.h"
#include "MonsterAIController.h"
#include "Engine/DamageEvents.h"

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

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PatrolOrigin = this->GetActorLocation();

    AnimInstance = GetMesh()->GetAnimInstance();
}

//TODO: 아 맘에 안들어 겁나 화나는 그런 구조네...
// BT_Attack 
// -> AEnemyCharacter::Attack() 
// -> Play Montage 
// -> Raise ApplyDamage 
// -> AEnemyCharacter::OnNotifyApplyDamage
// -> BT_Attack.OnApplyDamage;

void AEnemyCharacter::OnNotifyApplyDamage()
{
    FDamageEvent DummyDelegate;
    this->AttackTarget->TakeDamage(Damage
        , DummyDelegate
        , this->GetController()
        , this);
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

float AEnemyCharacter::GetDamage()
{
    return Damage;
}

void AEnemyCharacter::SetAttackTarget(ACharacter* Target)
{
    AttackTarget = Target;
}

EAttackAnimationState AEnemyCharacter::GetAttackAnimationeState()
{
    return AttackAnimationeState;
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
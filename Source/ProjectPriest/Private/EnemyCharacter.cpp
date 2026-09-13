// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "IngamePlayerController.h"
#include "PlayerCharacter.h"
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
	AttackRange = 0; //공격 사거리
	MinimumDamage = 1.0f;//몬스터가 받는 최소피해

	PatrolRadius = 1000.0f;
}


void AEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    if (!FMath::IsFinite(DamageAmount) || DamageAmount <= 0.0f || Health <= 0.0f || bIsDead || IsActorBeingDestroyed())
    {
        return 0.0f;
    }
    const float ActualDamage = FMath::Min(Health, FMath::Max(0.0f, FMath::Max(DamageAmount - Defense, MinimumDamage)));
    if (ActualDamage <= 0.0f) return 0.0f;
    Health -= ActualDamage;
    if (AIngamePlayerController* AttackingController = Cast<AIngamePlayerController>(EventInstigator))
    {
        AttackingController->ClientNotifyHitConfirmed(ActualDamage, GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
        if (Health <= 0.0f) AttackingController->ClientNotifyEnemyKilled();
    }
	UE_LOG(LogTemp, Warning, TEXT("몬스터가 받은 데미지: %.1f / 몬스터 현재 HP: %.1f"), ActualDamage, Health);
	if (Health <= 0)
	{
		Die();
	}
	return ActualDamage;
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

float AEnemyCharacter::GetAttackRange()
{
	return AttackRange;
}

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
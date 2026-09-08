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
	//추후 포인터를 플레이어캐릭터로 변경 예정
	//공통 공격 로직

    //Play Attack animation
    //-> DamageEven (데미지 주기)
    //-> 애니메이션 종료 대기(Abort or Finished)
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
}

void AEnemyCharacter::PlayAttackAnimation()
{
    
}

void AEnemyCharacter::OnApplyDamage(int SequenceNumber)
{
}

void AEnemyCharacter::OnAttackAnimationFinished(const int Reson)
{
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
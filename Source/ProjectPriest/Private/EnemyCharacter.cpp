// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
//추후 플레이어 캐릭터 인클루드  
//#include "PlayerCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//스탯 초기화
	Health = 100.0f;
	Damage = 10.0f;
	Defense = 5.0f;
	MinimumDamage = 1.0f;
}


void AEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
	//추후 포인터를 플레이어캐릭터로 변경 예정
	//공통 공격 로직
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
	
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


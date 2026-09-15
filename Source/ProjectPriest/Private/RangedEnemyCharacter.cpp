// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemyCharacter.h"
#include "Projectile.h"

ARangedEnemyCharacter::ARangedEnemyCharacter()
{
}

void ARangedEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
	Super::Attack(PlayerCharacter);
	//원거리 공격 로직 구현

    SpawnBullet();

    AttackAnimationeState = EAttackAnimationState::InProgress;

    GetWorld()->GetTimerManager()
        .SetTimer(AttackDelayTimerHandler
            , this
            , &ARangedEnemyCharacter::OnAttackDelayTimer
            , AttackDelay
            , false);
}

void ARangedEnemyCharacter::SpawnBullet() const
{
    FActorSpawnParameters SpawnParameter;
    SpawnParameter.Owner = const_cast<ARangedEnemyCharacter*>(this);
    SpawnParameter.Instigator = const_cast<ARangedEnemyCharacter*>(this);

    FRotator ProjectileRotation = GetActorForwardVector().Rotation();
    AProjectile* NewProjectile 
        = GetWorld()->SpawnActor<AProjectile>(Bullet
            , GetActorLocation()
            , ProjectileRotation
            , SpawnParameter);
}

void ARangedEnemyCharacter::OnAttackDelayTimer()
{
    AttackAnimationeState = EAttackAnimationState::Finished;
}

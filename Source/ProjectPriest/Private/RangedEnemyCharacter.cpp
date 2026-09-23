// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemyCharacter.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

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

void ARangedEnemyCharacter::Die()
{
    Super::Die();

    OnDeath();

    if (DeathParticle)
    {
        UParticleSystemComponent* Particle =
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                DeathParticle,
                GetActorLocation(),
                GetActorRotation()
            );

        if (Particle)
        {
            GetWorldTimerManager().SetTimer(
                DestroyParticle,
                [Particle]()
                {
                    if (IsValid(Particle))
                    {
                        Particle->DeactivateSystem();
                        Particle->DestroyComponent();
                    }
                },
                ParticleTime,
                false
            );
        }

    }
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

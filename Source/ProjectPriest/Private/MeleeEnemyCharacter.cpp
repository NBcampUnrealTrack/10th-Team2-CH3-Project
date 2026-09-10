#include "MeleeEnemyCharacter.h"
#include "JUtility.h"
#include <system_error>

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
    : SessionNames({
         "Attack0"
        ,"Attack1"
        ,"Attack2" })
{
}

void AMeleeEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
    Super::Attack(PlayerCharacter);

    JASSERT(IsValid(MontageToPlaying), "Montage is null or not setted");
    float MontagePlayResult = AnimInstance->Montage_Play(MontageToPlaying, 1.0f);
    AnimInstance->Montage_JumpToSection(GetRandomSessionName(), MontageToPlaying);

    if (MontagePlayResult <= 0.001f)
    {
        //TODO: Error what to do
        AttackAnimationeState = EAttackAnimationState::Error;
        JError("Montage play failed");
        return;
    }

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AMeleeEnemyCharacter::OnMontageEnded);

    AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlaying);
    AttackAnimationeState = EAttackAnimationState::InProgress;
}


void AMeleeEnemyCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        AttackAnimationeState = EAttackAnimationState::Interrupted;
    }
    else
    {
        AttackAnimationeState = EAttackAnimationState::Finished;
    }
}

const FName& AMeleeEnemyCharacter::GetRandomSessionName()
{
    int32 RandomIndex = FMath::RandRange(0, SessionNames.Num() - 1 );
    return SessionNames[RandomIndex];
}

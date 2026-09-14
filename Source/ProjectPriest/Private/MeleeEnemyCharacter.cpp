#include "MeleeEnemyCharacter.h"
#include "JUtility.h"
#include <system_error>
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
    : SessionNames({
         "Attack0"
        ,"Attack1"
        ,"Attack2" })
{
    LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
    RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));

    LeftHandCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
    RightHandCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));

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

void AMeleeEnemyCharacter::OnNotifyApplyDamage()
{
    TSet<AActor*> OverlappingActors;
    LeftHandCollision->GetOverlappingActors(OverlappingActors);
    RightHandCollision->GetOverlappingActors(OverlappingActors);
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("=== OnNotifyApplyDamage ===")
    );
    for (AActor* Actor : OverlappingActors) {
        if (Actor && Actor->ActorHasTag("Player")) {
            UGameplayStatics::ApplyDamage(
                Actor,
                Damage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("=== APPLY DAMAGE TO %s ==="),
                *Actor->GetName()
            );
        }
    }
}

const FName& AMeleeEnemyCharacter::GetRandomSessionName()
{
    int32 RandomIndex = FMath::RandRange(0, SessionNames.Num() - 1 );
    RandomSessionName = SessionNames[RandomIndex];
    return RandomSessionName;
}

#include "Sevarog.h"
#include "Components/SkeletalMeshComponent.h"
#include "JUtility.h"
#include "MvcControl.h"
#include "SpawnActor.h"
#include "Engine/DamageEvents.h"
#include "MvcModel.h"
#include "MonsterData.h"
#include "GlobalConst.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <IngameGameMode.h>

// Sets default values
ASevarog::ASevarog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASevarog::BeginPlay()
{
	Super::BeginPlay();
    JASSERT(IsValid(HudClass), "HudClass is not valid");

    USevarogHud* HudInstance = CreateWidget<USevarogHud>(GetWorld(), HudClass, TEXT("Sevarog Hud"));
    JASSERT(IsValid(HudInstance), "HudInstance is not valid");

	HudInstance->AddToViewport(FGlobalConst::FUiZOrder::SEVAROG_HUD_ZORDER);
	
	//Find Player
	AActor* FindActor =  UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	JASSERT(IsValid(FindActor), "FindActor is not valid");
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(FindActor);
	JASSERT(IsValid(PlayerCharacter), "PlayerCharacter is not valid");
	
	SetAttackTarget(PlayerCharacter);
	
	
	Phase = 0;
	InvokePropertyChanged(StaticCast<uint8>(ESevarogPropertyName::Health));	
}

// Called every frame
void ASevarog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASevarog::ApplyDamage()
{
	JASSERT(IsValid(AttackTarget), "%hs There is no target", __FUNCTION__);
	
	float SqrMeleeAttackRange = MeleeAttackRange * MeleeAttackRange;
	float SqrDistance = FVector::DistSquared (GetActorLocation(), AttackTarget->GetActorLocation());
	if (SqrDistance > SqrMeleeAttackRange)
	{
		JLog("%hs Target is out of range", __FUNCTION__);
		return;
	}
	
	FDamageEvent DamageEvent;
	AttackTarget->TakeDamage(Damage
		, DamageEvent
		, GetController()
		, this);
}

void ASevarog::IncreasePhase()
{
	Phase++;
}

int ASevarog::GetPhase()
{
	return Phase;
}

bool ASevarog::GetNovaCastingStarted()
{
	return bNovaCastingStarted;
}

void ASevarog::SetNovaCastingStarted(bool bIsStarted)
{
	bNovaCastingStarted = bIsStarted;
	
	InvokePropertyChanged(StaticCast<uint8>(ESevarogPropertyName::NovaCastingStarted));	
}

FDelegateHandle ASevarog::AddListener(UMvcControl* Control)
{
	return Delegate.AddUObject(Control, &UMvcControl::HandleModelChanged);
}

void ASevarog::RemoveListener(FDelegateHandle Handle)
{
	Delegate.Remove(Handle);
}

void ASevarog::InvokePropertyChanged(uint8 PropertyName)
{
	Delegate.Broadcast(this, PropertyName);
}

//TODO: EnemyCharacter에서 가져왔음 이거 나눠야 함
float ASevarog::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!FMath::IsFinite(DamageAmount) || DamageAmount <= 0.0f || Health <= 0.0f || bIsDead || IsActorBeingDestroyed())
	{
		return 0.0f;
	}
	const float ActualDamage = FMath::Min(Health, FMath::Max(0.0f, FMath::Max(DamageAmount - Defense, MinimumDamage)));
	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}
	Health -= ActualDamage;
	
	UE_LOG(LogTemp, Warning, TEXT("몬스터가 받은 데미지: %.1f / 몬스터 현재 HP: %.1f"), ActualDamage, Health);

	InvokePropertyChanged(StaticCast<uint8>(ESevarogPropertyName::Health));
	
    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
    AIngameGameMode* IngameGameMode = Cast<AIngameGameMode>(GameMode);
    JASSERT_RETURN(IsValid(IngameGameMode), -1.0f, "IngameGameMode is not valid");

    bool bIsEmptyHealth = Health <= 0.0f;
    bool bIsThirdPhase = Phase == THIRD_PHASE;

    //realy dead!
    if (bIsEmptyHealth && bIsThirdPhase)
    {
        IngameGameMode->OnBossDead();
    }
    
	return ActualDamage;
}

void ASevarog::Heal(float HealAmount)
{
	Super::Heal(HealAmount);
	
	InvokePropertyChanged(StaticCast<uint8>(ESevarogPropertyName::Health));
}



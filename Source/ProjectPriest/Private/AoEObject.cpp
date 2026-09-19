#include "AoEObject.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter.h"
#include "JUtility.h"
#include "Engine/DamageEvents.h"

// Sets default values
AAoEObject::AAoEObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    Vfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vfx"));

    SetRootComponent(SceneRoot);
    Vfx->SetupAttachment(SceneRoot);

    Duration = 10.0f;
    DamageDelay = 1.0f;
    EachDamage = 1.0f;
}

// Called when the game starts or when spawned
void AAoEObject::BeginPlay()
{
	Super::BeginPlay();

    StartTime = GetWorld()->GetTimeSeconds();

    bIsStartDelay = true;
}

// Called every frame
void AAoEObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (IsPendingKillPending())
        return;
    
    float Now = GetWorld()->GetTimeSeconds();
    if (bIsStartDelay)
    {
        if(Now - StartTime >= DelayFromStart)
        {
            bIsStartDelay = false;
            NextDamageTime = Now;
        }
    }
    else
    {
        if (NextDamageTime <= Now)
        {
            //타겟이 적당한 거리로 피한 경우 데미지가 없을 수 있음
            if (!IsValid(TargetActor))
                return;

            JASSERT(IsValid(Owner), "Owner must be exist");

            AController* OwnerController = Owner->GetInstigatorController();
            JASSERT(IsValid(OwnerController), "OwnerController must be exist");

            FDamageEvent DamageEvent;
            TargetActor->TakeDamage(EachDamage, DamageEvent, OwnerController, this);
            NextDamageTime = Now + DamageDelay;
        }
    }
    
    float TotalDuration = Now - StartTime;
    if (TotalDuration >= Duration)
    {
        JLog("%s Duration over will be destroy", *GetActorNameOrLabel());
        Destroy();
    }
}

void AAoEObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    JASSERT(IsValid(PlayerCharacter), "%s is not playercharacter", *OtherActor->GetName());

    TargetActor = PlayerCharacter;
}

void AAoEObject::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(TargetActor == OtherActor)
    {
        TargetActor = nullptr;
    }
}



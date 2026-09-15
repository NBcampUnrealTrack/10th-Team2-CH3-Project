#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "JUtility.h"
#include "PlayerCharacter.h"
#include "Engine/DamageEvents.h"
// Sets default values
AProjectile::AProjectile()

{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    Vfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Vfx"));
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

    SetRootComponent(SceneRoot);
    StaticMesh->SetupAttachment(SceneRoot);
    Vfx->SetupAttachment(SceneRoot);
    Collision->SetupAttachment(SceneRoot);    
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);
	Collision->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnedTime = GetWorld()->TimeSeconds;
	ReservedDestroying = false;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ReservedDestroying)
	{
		return;
	}
	
    FVector NextVelocity = GetActorForwardVector() * MoveSpeed * DeltaTime;
    
    AddActorWorldOffset(NextVelocity);
	
	float Now = GetWorld()->GetTimeSeconds();
	if (Now - SpawnedTime >= LifeTime)
	{
		Destroy();
		ReservedDestroying = true;
	}
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    JLog("OnOverlapBegin");
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!IsValid(PlayerCharacter))
	{
		JLog("%s is not PlayerCharacter", *OtherActor->GetName());
		return;
	}
	
	JLog("Take damage(%d) to player", Damage);
	FDamageEvent DamageEvent;
	PlayerCharacter->TakeDamage(Damage
		, DamageEvent
		, GetInstigator()->GetController()
		, GetOwner());
	
	Destroy();
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    JLog("OnOverlapEnd");
}

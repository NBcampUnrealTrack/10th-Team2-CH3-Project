#include "HolyGenerade.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AHolyGenerade::AHolyGenerade()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AHolyGenerade::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AHolyGenerade::OnItemEndOverlap);
}

void AHolyGenerade::SetIsThrown(bool bThrown)
{
	bIsThrown = bThrown;
}

FName AHolyGenerade::GetItemType() const {
	return ItemType;
}

void AHolyGenerade::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!bIsThrown)
	{
		return;
	}

	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	ActivateItem(OtherActor);
}

void AHolyGenerade::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void AHolyGenerade::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AHolyGenerade::Explode,
		ExplosionDelay,
		false);

	bHasExploded = true;
}

void AHolyGenerade::Explode() {
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors) {
		if (Actor && Actor->ActorHasTag("Monster")) {
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}

	DestroyItem();
}

void AHolyGenerade::DestroyItem() {
	Destroy();
}
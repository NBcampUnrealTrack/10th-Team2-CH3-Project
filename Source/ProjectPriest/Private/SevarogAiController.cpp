#include "SevarogAiController.h"
#include "EnemyCharacter.h"
#include "JUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sevarog.h"


// Sets default values
ASevarogAiController::ASevarogAiController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HealthRateKeyName = TEXT("HealthRate");
}

void ASevarogAiController::OnHealthRateChanged(float HealthRate)
{
	GetBlackboardComponent()->SetValueAsFloat(HealthRateKeyName, HealthRate);
}

// Called when the game starts or when spawned
void ASevarogAiController::BeginPlay()
{
	Super::BeginPlay();
	
	ASevarog* Sevarog = Cast<ASevarog>(GetPawn());
	JASSERT(IsValid(Sevarog), "Sevarog is not valid");
	
	GetBlackboardComponent()->SetValueAsFloat(HealthRateKeyName
		, Sevarog->GetHealthRate());
	
	Sevarog->GetHealthRateDeletate()
		.AddDynamic(this, &ASevarogAiController::OnHealthRateChanged);
}

// Called every frame
void ASevarogAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


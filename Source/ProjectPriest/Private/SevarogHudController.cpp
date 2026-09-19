#include "SevarogHudController.h"
#include "SevarogHud.h"
#include "Sevarog.h" 
#include "Kismet/GameplayStatics.h"
#include "JUtility.h"

void USevarogHudController::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
	Super::HandleViewEvent(InView, EventType, Parameter);
}

void USevarogHudController::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
	ASevarog* Sevarog = Cast<ASevarog>(InModel);
	JASSERT(IsValid(Sevarog)
		, "%s is not Sevarog"
		, *Sevarog->GetName());
	
	ESevarogPropertyName CastedPropertyName = StaticCast<ESevarogPropertyName>(PropertyName);
	FText PropertyText = StaticEnum<ESevarogPropertyName>()
		->GetDisplayNameTextByValue(StaticCast<int64>(CastedPropertyName));
	
	switch (CastedPropertyName)
	{
		case ESevarogPropertyName::Health:
		{
			float HealthRate = Sevarog->GetHealthRate();
			GetView<USevarogHud>()->SetHealthRate(HealthRate);
			break;
		}
		
		default:
			JError("%s not supported yet" 
				, *PropertyText.ToString());
		break;		
	}
	
}

USevarogHudController* USevarogHudController::Create(IMvcView* View, UWorld* World)
{
	USevarogHud* BossHud = StaticCast<USevarogHud*>(View);
	
	USevarogHudController* NewController = NewObject<USevarogHudController>();
	NewController->SetView(View);
	
	ASevarog* Sevarog = Cast<ASevarog>(UGameplayStatics::GetActorOfClass(World, ASevarog::StaticClass()));
	Sevarog->AddListener(NewController);
	
	return NewController;
}

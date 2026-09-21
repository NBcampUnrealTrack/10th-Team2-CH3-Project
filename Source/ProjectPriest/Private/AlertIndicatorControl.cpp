#include "AlertIndicatorControl.h"
#include "AlertIndicator.h"
#include "Sevarog.h"
#include "JUtility.h"
#include <Kismet/GameplayStatics.h>

void UAlertIndicatorControl::HandleViewEvent(IMvcView* InView, EViewEventType EventType, UEventParameterBase* Parameter)
{
	Super::HandleViewEvent(InView, EventType, Parameter);

	//딱히 뷰에서 뭔가 올 일은 없다.
}

void UAlertIndicatorControl::HandleModelChanged(IMvcModel* InModel, uint8 PropertyName)
{
	ASevarog* Model = Cast<ASevarog>(InModel);

	ESevarogPropertyName SevarogPropertyName = StaticCast<ESevarogPropertyName>(PropertyName);
	switch (SevarogPropertyName)
	{
	case ESevarogPropertyName::NovaCastingStarted:
		{
			JLog("Nova castring started property changed");
			UAlertIndicator* View = GetView<UAlertIndicator>();
			if (Model->GetNovaCastingStarted())
			{
				View->Show();
			}
			else
			{
				View->Hide();
			}
			break;
		}

	default:
		//nothing todo
		break;
	}
}

UAlertIndicatorControl* UAlertIndicatorControl::Create(IMvcView* View, UWorld* World)
{
	UAlertIndicatorControl* NewControl = NewObject<UAlertIndicatorControl>();
	NewControl->SetView(View);

	ASevarog* Sevarog = Cast<ASevarog>(UGameplayStatics::GetActorOfClass(World, ASevarog::StaticClass()));
	Sevarog->AddListener(NewControl);

	return NewControl;
}

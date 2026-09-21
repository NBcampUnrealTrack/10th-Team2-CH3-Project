#include "AlertIndicator.h"

#include <Kismet/GameplayStatics.h>

#include "AlertIndicatorControl.h"
#include "JUtility.h"

IMPLEMENT_VIEW_DEFAULT_INTERFACE(UAlertIndicator, UAlertIndicatorControl)

void UAlertIndicator::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UAlertIndicator::Show()
{
	SetVisibility(ESlateVisibility::Visible);
	
	if (IsValid(AlertSound))
	{
		UGameplayStatics::PlaySound2D(this, AlertSound.Get());
	}
}

void UAlertIndicator::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

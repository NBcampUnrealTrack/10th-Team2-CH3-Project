#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MvcView.h"
#include "MvcUtility.h"
#include "AlertIndicator.generated.h"


class USoundBase;

UCLASS()
class PROJECTPRIEST_API UAlertIndicator : public UUserWidget, public IMvcView
{
	GENERATED_BODY()

	DECLARE_VIEW_DEFAULT_INTERFACES();
	
public:
	virtual void NativeConstruct() override;
    void Show();
    void Hide();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="===Alert===|Properties")
    TObjectPtr<USoundBase> AlertSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Alert===|Properties")
    float Duration = 3.0f;
	
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PriestDamageNumberWidget.generated.h"

class UTextBlock;

// WBP에서 배치한 숫자 위젯에 피해량과 화면 위치를 전달한다.
UCLASS(Abstract)
class PROJECTPRIEST_API UPriestDamageNumberWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void InitializeDamage(float Amount, const FVector& Location);
protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> DamageText;
    UPROPERTY(EditDefaultsOnly, Category="Damage Number", meta=(ClampMin="0.1"))
    float Lifetime = 0.7f;
    UPROPERTY(EditDefaultsOnly, Category="Damage Number")
    float RiseDistance = 60.0f;
private:
    void RefreshText();
    FVector WorldLocation = FVector::ZeroVector;
    float Damage = 0.0f;
    float Age = 0.0f;
};

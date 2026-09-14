#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PriestDamageNumberWidget.generated.h"

class UTextBlock;

// 기본 숫자 표시를 제공하며, WBP 자식으로 스타일을 교체할 수 있다.
UCLASS()
class PROJECTPRIEST_API UPriestDamageNumberWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void InitializeDamage(float Amount, const FVector& Location);
protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

    UPROPERTY(meta=(BindWidgetOptional)) TObjectPtr<UTextBlock> DamageText;
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

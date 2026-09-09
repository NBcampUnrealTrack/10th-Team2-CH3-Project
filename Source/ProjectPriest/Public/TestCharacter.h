#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class PROJECTPRIEST_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATestCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

protected:
	// 권총 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 기본 공격
	UFUNCTION()
	void Attack(const FInputActionValue& value);
};

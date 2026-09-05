#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionInstance;
struct FInputActionValue;

UCLASS()
class PROJECTPRIEST_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void Move(const FInputActionInstance& InputValue);
    
    UFUNCTION()
    void Look(const FInputActionInstance& InputValue);

    UFUNCTION()
    void Attack(const FInputActionValue& value);

    UFUNCTION()
    void Throw(const FInputActionValue& value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<USpringArmComponent> SpringArm;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    float MovingSpeed = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    float RotationSpeed = 90.0f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Inputs")
    bool bShouldInvertX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Inputs")
    bool bShouldInvertY;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionInstance;
struct FInputActionValue;

USTRUCT(BlueprintType) struct  FSpeedConfig
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Accel;
};

UENUM(BlueprintType)
enum class EWalkingMode : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Sprint UMETA(DisplayName = "Srpint"),
};


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
    void OnMoveInputted(const FInputActionInstance& InputValue);
    
    UFUNCTION()
    void OnLookInputted(const FInputActionInstance& InputValue);
    
    UFUNCTION()
    void OnJumpInputted(const FInputActionInstance& InputValue);

    UFUNCTION()
    void OnSprintInputted(const FInputActionInstance& InputValue);    

    UFUNCTION()
    void OnAttackInputted(const FInputActionValue& value);

    UFUNCTION()
    void OnThrowInputted(const FInputActionValue& value);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void ChangeWalkingMode(EWalkingMode WalkingMode);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<USpringArmComponent> SpringArm;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    float MovingSpeed = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    float RotationSpeed = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    EWalkingMode Mode = EWalkingMode::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    TMap<EWalkingMode, FSpeedConfig> MovementConfigMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    float BaseDamage = 20.0f;
};
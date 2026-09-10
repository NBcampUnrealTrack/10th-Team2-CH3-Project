#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MvcEvents.h"
#include "MvcModel.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class AHolyGenerade;
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
class PROJECTPRIEST_API APlayerCharacter
    : public ACharacter
    , public IMvcModel
{
	GENERATED_BODY()

public:
    enum class PropertyName {
        CurrentHealth,
        MaxHealth
    };

public:
	// Sets default values for this character's properties
	APlayerCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual float TakeDamage(
        float DamageAmount,
        FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser
    ) override;

    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    /// <summary>
    /// 
    /// </summary>
    /// <param name="PropertyName">must be casted from enum type</param>
    virtual void InvokePropertyChanged(uint8 PropertyName) override;

    UFUNCTION(BlueprintCallable)
    int GetCurrentHealth();

    UFUNCTION(BlueprintCallable)
    int GetMaxHealth();

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

    void ResetThrowCoolTime();

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Properties")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===PlayerCharacter===|Properties")
    float CurrentHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Throw")
    TSubclassOf<AHolyGenerade> HolyGrenadeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Throw")
    FName RightHandSocketName = TEXT("hand_r");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Throw")
    float ThrowDistance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Throw")
    float ThrowForce = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Throw")
    float ThrowCoolTime = 5.0f;

protected:
    bool bCanThrow = true;

    FTimerHandle ThrowCoolTimeTimerHandle;
    
    FModelChangedDelegate Listeners;
};
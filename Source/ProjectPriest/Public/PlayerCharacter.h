#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PotionTypes.h"
#include "PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FPlayerCombatChanged);
DECLARE_MULTICAST_DELEGATE_OneParam(FCanInteractChangedDelegate, bool);

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class AHolyGenerade;
class AWeaponItem;
class UDataTable;
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

    virtual float TakeDamage(
        float DamageAmount,
        FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser
    ) override;

    FPlayerCombatChanged OnCombatChanged;
    float GetCurrentHealth() const { return CurrentHealth; }
    float GetMaxHealth() const { return MaxHealth; }
    AWeaponItem* GetEquippedWeapon() const { return WeaponInstance.Get(); }

    const FVector GetMuzzleLocation();
    /////////////////////////////////////
    /// START INTERACTION
    /////////////////////////////////////

    UFUNCTION()
    void OnSensorOverlapBegin(class UPrimitiveComponent* OverlappedComp
        , class AActor* OtherActor
        , class UPrimitiveComponent* OtherComp
        , int32 OtherBodyIndex
        , bool bFromSweep
        , const FHitResult& SweepResult);

    UFUNCTION()
    void OnSensorOverlapEnd(class UPrimitiveComponent* OverlappedComp
        , class AActor* OtherActor
        , class UPrimitiveComponent* OtherComp
        , int32 OtherBodyIndex);
    
    FCanInteractChangedDelegate GetOnCanInteractDelegate();
    /////////////////////////////////////
    /// END INTERACTION
    /////////////////////////////////////
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
    
    UFUNCTION()
    void OnInteractInputted(const FInputActionValue& value);

    UFUNCTION(BlueprintCallable, Category = "Items|Potion")
    EPotionUseResult TryUsePotion(FName ItemId);

    UFUNCTION(BlueprintCallable, Category = "Items|QuickSlot")
    EPotionUseResult TryUseQuickSlot(int32 SlotIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void ChangeWalkingMode(EWalkingMode WalkingMode);

    void ResetThrowCoolTime();

    void OnDeath();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<USpringArmComponent> SpringArm;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<UCameraComponent> Camera;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Components")
    TObjectPtr<USphereComponent> InteractionSensor;;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Weapon")
    TSubclassOf<AWeaponItem> WeaponClass;

    //주의 현재 무기 메시가 캐릭터 모델에 달려있음
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Weapon")
    TObjectPtr<AWeaponItem> WeaponInstance;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Interacting")
    FCanInteractChangedDelegate OnCanInteractChanged;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Interacting")
    bool bCanInteract;
    
    bool bCanThrow = true;

    bool bIsDead = false;

    FTimerHandle ThrowCoolTimeTimerHandle;

    UPROPERTY(
        EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Items|Potion"
    )
    TObjectPtr<UDataTable> PotionDefinitions;

private:
    bool bIsUsingPotion = false;
};
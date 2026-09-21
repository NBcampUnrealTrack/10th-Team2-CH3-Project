#include "PlayerCharacter.h"
#include "IngamePlayerController.h"
#include "IngameGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HolyGenerade.h"
#include "DrawDebugHelpers.h"
#include "JUtility.h"
#include "WeaponItem.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "IngameGameMode.h"
#include "IngameGameState.h"
#include "PriestInventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Templates/UnrealTemplate.h"
#include  "JUtility.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    
    InteractionSensor = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSensor"));
    InteractionSensor->SetupAttachment(RootComponent);
    
    InteractionSensor->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSensorOverlapBegin);
    InteractionSensor->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSensorOverlapEnd);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    CurrentHealth = MaxHealth;

    ChangeWalkingMode(EWalkingMode::Normal);

    //주의 현재 무기 메시가 캐릭터 모델에 달려있음
    if (WeaponClass)
    {
        WeaponInstance = GetWorld()->SpawnActor<AWeaponItem>(WeaponClass);
        if (IsValid(WeaponInstance))
        {
            WeaponInstance->SetOwner(this);
        }
    }
    OnCombatChanged.Broadcast();
    
    bCanInteract = false;
}

void APlayerCharacter::ChangeWalkingMode(EWalkingMode WalkingMode)
{
    //
    FSpeedConfig* Config = MovementConfigMap.Find(WalkingMode);
    if (nullptr == Config)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Map.Find Faield"));
        return;
    }

    Mode = WalkingMode;
    GetCharacterMovement()->MaxWalkSpeed = Config->Speed;
    GetCharacterMovement()->MaxAcceleration = Config->Accel;
}

void APlayerCharacter::ResetThrowCoolTime()
{
    bCanThrow = true;
}

void APlayerCharacter::OnDeath()
{
    if (bIsDead)
    {
        return;
    }

    bIsDead = true;

    //사망 전에 공격속도증가포션 효과가 남아있다면 정리
    GetWorldTimerManager().ClearTimer(AttackSpeedPotionTimerHandle);
    EndAttackSpeedPotionEffect();

    // 게임 모드 호출
    AIngameGameMode* IngameGameMode = GetWorld()->GetAuthGameMode<AIngameGameMode>();

    JASSERT(IngameGameMode, "IngameGameMode가 없습니다.");

    IngameGameMode->OnPlayerDead();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    //JASSERT(IsValid(EnhancedInput), "PlayerInputComponent is not UEnhancedInputComponent");

    AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(GetController());
    //JASSERT(IsValid(PlayerController), "GetController is not APlayerController");

    EnhancedInput->BindAction(
        PlayerController->GetFireAction(),
        ETriggerEvent::Started,
        this,
        &APlayerCharacter::OnAttackInputted

    );

    EnhancedInput->BindAction(
        PlayerController->GetMoveAction(),
        ETriggerEvent::Triggered,
        this,
        &APlayerCharacter::OnMoveInputted
    );

    EnhancedInput->BindAction(
        PlayerController->GetLookAction(),
        ETriggerEvent::Triggered,
        this,
        &APlayerCharacter::OnLookInputted
    );

    EnhancedInput->BindAction(
        PlayerController->GetSprintAction(),
        ETriggerEvent::Started,
        this,
        &APlayerCharacter::OnSprintInputted
    );

    EnhancedInput->BindAction(
        PlayerController->GetJumpAction(),
        ETriggerEvent::Triggered,
        this,
        &APlayerCharacter::OnJumpInputted
    );

    EnhancedInput->BindAction(
        PlayerController->GetThrowAction(),
        ETriggerEvent::Started,
        this,
        &APlayerCharacter::OnThrowInputted
    );
    
    
    EnhancedInput->BindAction(
        PlayerController->GetInteractAction(),
        ETriggerEvent::Started,
        this,
        &APlayerCharacter::OnInteractInputted
    );
}

float APlayerCharacter::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    if (const AIngameGameState* State = GetWorld()->GetGameState<AIngameGameState>())
    {
        if (State->HasStageCleared())
        {
            return 0.0f;
        }
    }
    const float ActualDamage = Super::TakeDamage(
        DamageAmount,
        DamageEvent,
        EventInstigator,
        DamageCauser
    );

    const float PreviousHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
    if (CurrentHealth < PreviousHealth)
    {
        if (AIngamePlayerController* OwningController = Cast<AIngamePlayerController>(GetController()))
        {
            OwningController->ClientNotifyPlayerDamaged(this);
        }
    }
    OnCombatChanged.Broadcast();

    JLog("플레이어 데미지: %.1f / 현재 HP: %.1f", ActualDamage, CurrentHealth);

    if (CurrentHealth <= 0.0f)
    {
        OnDeath();
    }

    return ActualDamage;
}

float APlayerCharacter::GetCurrentHealth() const
{
    return CurrentHealth;
}

float APlayerCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

AWeaponItem* APlayerCharacter::GetEquippedWeapon() const
{
    return WeaponInstance.Get();
}

const FVector APlayerCharacter::GetMuzzleLocation()
{
    const FName SocketName = TEXT("gun_pinSocket");

    if (!GetMesh()->DoesSocketExist(SocketName))
    {
        JLog("총구 Socket이 없습니다.");
        return FVector::Zero();
    }

    return GetMesh()->GetSocketLocation(SocketName);
}

void APlayerCharacter::OnSensorOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    bCanInteract = true;
    
    JLog("CanInteract : True");
    OnCanInteractChanged.Broadcast(bCanInteract);
}

void APlayerCharacter::OnSensorOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bCanInteract = false;
    
    JLog("CanInteract : False");
    OnCanInteractChanged.Broadcast(bCanInteract);
}

FCanInteractChangedDelegate APlayerCharacter::GetOnCanInteractDelegate()
{
    return OnCanInteractChanged;
}

void APlayerCharacter::OnMoveInputted(const FInputActionInstance& InputValue)
{
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Move"));

    FVector2D Input = InputValue.GetValue().Get<FVector2D>();
    //TODO: Camera의 Forward로 할지 Actor의 FOrawrd로 할지 테스트해보고 정해야함
    FVector Forward = GetActorForwardVector();
    FVector Right = GetActorRightVector();
    FVector Direction = (Forward * Input.Y) + (Right * Input.X);
    Direction.Normalize();

    GetCharacterMovement()->AddInputVector(Direction);
}

void APlayerCharacter::OnLookInputted(const FInputActionInstance& InputValue)
{
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Look"));

    FVector2D Input = InputValue.GetValue().Get<FVector2d>();

    AddControllerPitchInput(Input.Y * RotationSpeed);
    AddControllerYawInput(Input.X * RotationSpeed);
}

void APlayerCharacter::OnJumpInputted(const FInputActionInstance& InputValue)
{
    if (CanJump())
    {
        Super::Jump();
    }
}

void APlayerCharacter::OnAttackInputted(const FInputActionValue& value)
{
    //JLog("OnAttackInputted");

    if (!IsValid(WeaponInstance))
    {
        return;
    }

    if (WeaponInstance->CanFire())
    {
        WeaponInstance->Attack();
    }
    
    if(WeaponInstance->GetCurrentAmmo() == 0)
    {
        if (!WeaponInstance->CanReload())
        {
            JLog("재장전 불가능");
            return;
        }

        WeaponInstance->Reload();
    }
    
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Attack"));

    /*
    const FName SocketName = TEXT("gun_pinSocket");

    if (!GetMesh()->DoesSocketExist(SocketName))
    {
        //UE_LOG(LogTemp, Warning, TEXT("총구 Socket이 없습니다."));
        return;
    }

    AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    // 카메라 기준으로 조준점 찾기
    FVector CameraStart = PlayerController->PlayerCameraManager->GetCameraLocation();
    FVector CameraForward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
    FVector CameraEnd = CameraStart + CameraForward * 10000.0f;

    FHitResult CameraHitResult;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    GetWorld()->LineTraceSingleByChannel(
        CameraHitResult,
        CameraStart,
        CameraEnd,
        ECC_Visibility,
        QueryParams
    );

    // 조준점
    FVector AimPoint;

    if (CameraHitResult.bBlockingHit)
    {
        AimPoint = CameraHitResult.ImpactPoint;
    }
    else
    {
        AimPoint = CameraEnd;
    }

    const FVector MuzzleLocation = GetMesh()->GetSocketLocation(SocketName);
    const FVector ShotDirection = (AimPoint - MuzzleLocation).GetSafeNormal();
    const FVector ShotEnd = MuzzleLocation + ShotDirection * 10000.0f;

    FHitResult ShotHitResult;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        ShotHitResult,
        MuzzleLocation,
        ShotEnd,
        ECC_Visibility,
        QueryParams
    );

    if (bHit)
    {
        AActor* HitActor = ShotHitResult.GetActor();

        if (HitActor)
        {
            // 총구에서 피격 지점까지 디버그 라인 생성
            DrawDebugLine(
                GetWorld(),
                MuzzleLocation,
                ShotHitResult.ImpactPoint,
                FColor::Green,
                false,
                1.0f,
                0,
                2.0f
            );

            // 데미지 적용
            UGameplayStatics::ApplyDamage(
                HitActor,
                BaseDamage,
                PlayerController,
                this,
                nullptr
            );
        }
    }
    else
    {
        // 총구에서 최대 사거리까지 디버그 라인 생성
        DrawDebugLine(
            GetWorld(),
            MuzzleLocation,
            ShotEnd,
            FColor::Green,
            false,
            1.0f,
            0,
            2.0f
        );
    }
    */
}

void APlayerCharacter::OnThrowInputted(const FInputActionValue& value)
{
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Throw"));

    if (!bCanThrow)
    {
        return;
    }

    bCanThrow = false;

    if (!HolyGrenadeClass)
    {
        return;
    }

    const FName SocketName = RightHandSocketName;

    if (!GetMesh()->DoesSocketExist(SocketName))
    {
        //UE_LOG(LogTemp, Warning, TEXT("오른손 Socket이 없습니다."));
        return;
    }

    // 오른손 위치
    const FVector HandLocation = GetMesh()->GetSocketLocation(SocketName);

    // 캐릭터가 바라보는 방향
    const FVector Forward = GetActorForwardVector();
    // 카메라가 바라보는 방향
    // const FVector Forward = Camera->GetForwardVector();

    // 캐릭터의 오른손보다 조금 앞에서 생성
    const FVector SpawnLocation = HandLocation + Forward * ThrowDistance;

    FRotator SpawnRotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;

    // 스폰
    AHolyGenerade* HolyGrenade =
        GetWorld()->SpawnActor<AHolyGenerade>(
            HolyGrenadeClass,
            SpawnLocation,
            SpawnRotation,
            SpawnParams
        );

    if (!HolyGrenade)
    {
        return;
    }

    // 투척
    HolyGrenade->Throw(Forward, ThrowForce);

    GetWorld()->GetTimerManager().SetTimer(
        ThrowCoolTimeTimerHandle,
        this,
        &APlayerCharacter::ResetThrowCoolTime,
        ThrowCoolTime,
        false
    );
}

void APlayerCharacter::OnInteractInputted(const FInputActionValue& value)
{
    JLog("Intract inputted");
    
    // The GameMode checks the actual objective overlap; the generic sensor flag
    // can be cleared when another overlapping actor leaves the sensor.
        
    
    AIngameGameMode* IngameGameMode 
        = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());
    
    JASSERT(IsValid(IngameGameMode), "Game mode is not IngameGameMode or nullptr");
    
    IngameGameMode->OnOpenBossRoomDoor(this);
}

void APlayerCharacter::OnSprintInputted(const FInputActionInstance& InputValue)
{
    switch (Mode)
    {
    case EWalkingMode::Normal:
        ChangeWalkingMode(EWalkingMode::Sprint);
        break;
    case EWalkingMode::Sprint:
        ChangeWalkingMode(EWalkingMode::Normal);
        break;
    default:
        break;
    }
}

EPotionUseResult APlayerCharacter::TryUsePotion(FName ItemId)
{
    if (bIsUsingPotion || IsActorBeingDestroyed())
    {
        return EPotionUseResult::Unavailable;
    }

    TGuardValue<bool> UseGuard(bIsUsingPotion, true);

    UGameInstance* GameInstance = GetGameInstance();

    JASSERT_RETURN((IsValid(GameInstance)), EPotionUseResult::Unavailable, "GameInstance가 없습니다.");

    UPriestInventorySubsystem* Inventory =
        GameInstance->GetSubsystem<UPriestInventorySubsystem>();

    JASSERT_RETURN((IsValid(Inventory)), EPotionUseResult::Unavailable, "InventorySubsystem이 없습니다.");

    FHealthPotionData HealthData;
    if (Inventory->TryGetHealthPotionData(ItemId, HealthData))
    {
        if (!FMath::IsFinite(CurrentHealth)
            || !FMath::IsFinite(MaxHealth)
            || MaxHealth <= 0.0f)
        {
            JError("잘못된 체력 설정. CurrentHealth=%f, MaxHealth=%f", CurrentHealth, MaxHealth);
            return EPotionUseResult::Unavailable;
        }

        if (bIsDead || CurrentHealth <= 0.0f)
        {
            return EPotionUseResult::Dead;
        }

        if (CurrentHealth >= MaxHealth)
        {
            return EPotionUseResult::FullHealth;
        }

        if (Inventory->GetQuantity(ItemId) < 1)
        {
            return EPotionUseResult::NotOwned;
        }
        const float PreviousHealth = CurrentHealth;
        const float MissingHealth = MaxHealth - CurrentHealth;
        const float ActualHeal =
            FMath::Min(HealthData.HealAmount, MissingHealth);

        const float NewHealth =
            FMath::Min(CurrentHealth + ActualHeal, MaxHealth);

        if (NewHealth <= PreviousHealth)
        {
            return EPotionUseResult::Unavailable;
        }

        CurrentHealth = NewHealth;

        if (!Inventory->RemoveItem(ItemId, 1))
        {
            CurrentHealth = PreviousHealth;
            return EPotionUseResult::NotOwned;
        }

        OnCombatChanged.Broadcast();

        return EPotionUseResult::Success;
    }

    FAttackSpeedUpPotionData AttackSpeedData;
    if (Inventory->TryGetAttackSpeedUpPotionData(ItemId, AttackSpeedData))
    {
        if (bIsDead || CurrentHealth <= 0.0f)
        {
            return EPotionUseResult::Dead;
        }

        if (Inventory->GetQuantity(ItemId) < 1)
        {
            return EPotionUseResult::NotOwned;
        }

        if (!Inventory->RemoveItem(ItemId, 1))
        {
            return EPotionUseResult::NotOwned;
        }

        AttackSpeedMultiplier = AttackSpeedData.AttackSpeedMultiplier;

        GetWorldTimerManager().SetTimer(
            AttackSpeedPotionTimerHandle,
            this,
            &APlayerCharacter::EndAttackSpeedPotionEffect,
            AttackSpeedData.Duration,
            false
        );

        return EPotionUseResult::Success;
    }

    return EPotionUseResult::InvalidPotion;
}

EPotionUseResult APlayerCharacter::TryUseQuickSlot(
    int32 SlotIndex
)
{
    UGameInstance* GameInstance = GetGameInstance();

    JASSERT_RETURN((IsValid(GameInstance)), EPotionUseResult::Unavailable, "GameInstance가 없습니다.");

    UPriestInventorySubsystem* Inventory =
        GameInstance->GetSubsystem<UPriestInventorySubsystem>();

    JASSERT_RETURN((IsValid(Inventory)), EPotionUseResult::Unavailable, "InventorySubsystem이 없습니다.");

    const FName ItemId =
        Inventory->GetQuickSlotItemId(SlotIndex);

    if (ItemId.IsNone())
    {
        return EPotionUseResult::Unavailable;
    }

    return TryUsePotion(ItemId);
}

float APlayerCharacter::GetAttackSpeedMultiplier() const
{
    return AttackSpeedMultiplier;
}

void APlayerCharacter::EndAttackSpeedPotionEffect()
{
    AttackSpeedMultiplier = 1.0f;
    AttackSpeedPotionTimerHandle.Invalidate();
    JLog("포션 지속시간 종료");
}

#include "PlayerCharacter.h"
#include "IngamePlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HolyGenerade.h"

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
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    CurrentHealth = MaxHealth;

    ChangeWalkingMode(EWalkingMode::Normal);
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

// Called every frame
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
}

float APlayerCharacter::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(
        DamageAmount,
        DamageEvent,
        EventInstigator,
        DamageCauser
    );

    CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);

    //UE_LOG(LogTemp, Warning, TEXT("플레이어 데미지: %.1f / 현재 HP: %.1f"), ActualDamage, CurrentHealth);

    if (CurrentHealth <= 0.0f)
    {
        // 사망 함수 호출
    }

    return ActualDamage;
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
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Attack"));

    const FName SocketName = TEXT("gun_pinSocket");

    if (!GetMesh()->DoesSocketExist(SocketName))
    {
        //UE_LOG(LogTemp, Warning, TEXT("총구 Socket이 없습니다."));
        return;
    }

    const FVector Start = GetMesh()->GetSocketLocation(SocketName);
    const FVector Forward = GetMesh()->GetSocketRotation(SocketName).Vector();

    const FVector End = Start + Forward * 10000.0f;

    FHitResult Hit;

    GetWorld()->LineTraceSingleByChannel(
        Hit,
        Start,  
        End,
        ECC_Visibility
    );

    if (Hit.bBlockingHit)
    {
        UGameplayStatics::ApplyDamage(
            Hit.GetActor(),
            BaseDamage,
            GetController(),
            this,
            UDamageType::StaticClass()
        );
    }
}

void APlayerCharacter::OnThrowInputted(const FInputActionValue& value)
{
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Throw"));

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
#include "PlayerCharacter.h"
#include "IngamePlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "IngamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
        PlayerController->GetLookAction(),
        ETriggerEvent::Started,
        this,
        &APlayerCharacter::OnThrowInputted
    );
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
}

void APlayerCharacter::OnThrowInputted(const FInputActionValue& value)
{
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("APlayerCharacter::Throw"));
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

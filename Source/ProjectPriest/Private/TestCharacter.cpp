#include "TestCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestPlayerController.h"
#include "Kismet/GameplayStatics.h"

ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	BaseDamage = 20.0f;
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController())) {
			if (PlayerController->FireAction) {
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Started,
					this,
					&ATestCharacter::Attack
				);
			}
		}
	}
}

void ATestCharacter::Attack(const FInputActionValue& value) {
	const FName SocketName = TEXT("gun_pinSocket");
	if (!GetMesh()->DoesSocketExist(SocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("총구 Socket이 없습니다."));
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


#include "WeaponItem.h"
#include "IngamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "PlayerCharacter.h"
#include "JUtility.h"

AWeaponItem::AWeaponItem()
{
	ItemType = TEXT("Weapon");
}

bool AWeaponItem::IsReloading() const
{
	return bIsReloading;
}

void AWeaponItem::Attack()
{
	if (!CanFire())
	{
		return;
	}

	CurrentAmmo--;
    OnAmmoChanged.Broadcast();

	bCanFire = false;

	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&AWeaponItem::ResetFire,
		FireInterval,
		false
	);

	AActor* OwnerActor = GetOwner();

	if (!OwnerActor)
	{
		return;
	}

	AIngamePlayerController* PlayerController = Cast<AIngamePlayerController>(OwnerActor->GetInstigatorController());

	if (!PlayerController)
	{
		return;
	}

	// FPS
	//PerformTraceFPS(PlayerController, OwnerActor);

	// TPS
	PerformTraceTPS(PlayerController, OwnerActor);

	JLog("현재 탄약: %d / %d", CurrentAmmo, ReserveAmmo);
}

void AWeaponItem::Reload()
{
	if (!CanReload())
	{
		return;
	}

    JLog("재장전 시작");

	// 재장전 시작
	bIsReloading = true;

	GetWorldTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&AWeaponItem::CompleteReload,
		ReloadTime,
		false
	);

	//UE_LOG(LogTemp, Warning, TEXT("재장전"));
}

bool AWeaponItem::CanFire() const
{
	return bCanFire && !bIsReloading && CurrentAmmo > 0;
}

bool AWeaponItem::CanReload() const
{
	// 재장전 중이면 false
	if (bIsReloading)
	{
		return false;
	}

	// 현재 탄창이 가득 차 있으면 false
	if (CurrentAmmo >= MagazineSize)
	{
		return false;
	}

	// 예비 탄약이 없으면 false
	if (ReserveAmmo <= 0)
	{
		return false;
	}

	return true;
}

int AWeaponItem::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

void AWeaponItem::CompleteReload()
{
	// 재장전에 필요한 탄약 수
	const int32 NeededAmmo = MagazineSize - CurrentAmmo;

	// 실제로 재장전할 수 있는 탄약 수
	const int32 ReloadAmount = FMath::Min(NeededAmmo, ReserveAmmo);

	CurrentAmmo += ReloadAmount;
	ReserveAmmo -= ReloadAmount;
    OnAmmoChanged.Broadcast();

	bIsReloading = false;

    JLog("재장전 완료")
}

void AWeaponItem::ResetFire()
{
	bCanFire = true;
}

void AWeaponItem::PerformTraceFPS(
	AIngamePlayerController* PlayerController,
	AActor* OwnerActor)
{
	if (!PlayerController || !OwnerActor)
	{
		return;
	}

	FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
	FVector Forward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
	FVector End = Start + Forward * Range;

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerActor);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			// 피격 지점까지 디버그 라인 생성
			DrawDebugLine(
				GetWorld(),
				Start,
				HitResult.ImpactPoint,
				FColor::Green,
				false,
				1.0f,
				0,
				2.0f
			);

			// 데미지 적용
			UGameplayStatics::ApplyDamage(
				HitActor,
				Damage,
				PlayerController,
				OwnerActor,
				nullptr
			);
		}
	}
	else
	{
		// 최대 사거리까지 디버그 라인 생성
		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Green,
			false,
			1.0f,
			0,
			2.0f
		);
	}
}

void AWeaponItem::PerformTraceTPS(
	AIngamePlayerController* PlayerController,
	AActor* OwnerActor)
{
	if (!PlayerController || !OwnerActor)
	{
		return;
	}

	// 카메라 기준으로 조준점 찾기
	FVector CameraStart = PlayerController->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
	FVector CameraEnd = CameraStart + CameraForward * Range;

	FHitResult CameraHitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerActor);

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

	// 총구에서 조준점으로 발사
	// 캐릭터 메시가 총을 가지고 있어서 이와 같은 모습이 됨...
	//FVector MuzzleLocation = GetActorLocation();
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
    FVector MuzzleLocation = PlayerCharacter->GetMuzzleLocation();
	FVector ShotDirection = (AimPoint - MuzzleLocation).GetSafeNormal();
	FVector ShotEnd = MuzzleLocation + ShotDirection * Range;

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

		if (HitActor && HitActor->ActorHasTag("Monster"))
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
				Damage,
				PlayerController,
				OwnerActor,
				nullptr
			);

			JLog("몬스터에게 %.1f 데미지를 입힘", Damage);
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
}

void AWeaponItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	Super::EndPlay(EndPlayReason);
}
#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "WeaponItem.generated.h"

DECLARE_MULTICAST_DELEGATE(FWeaponAmmoChanged);

class AIngamePlayerController;
class UPartManager;

UCLASS()
class PROJECTPRIEST_API AWeaponItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AWeaponItem();

	bool IsReloading() const;

    // Functions
    virtual void Attack();

    virtual void Reload();

    virtual bool CanFire() const;

    virtual bool CanReload() const;

    int GetCurrentAmmo() const;
    int32 GetReserveAmmo() const { return ReserveAmmo; }
    FText GetWeaponName() const { return WeaponName; }
    FWeaponAmmoChanged OnAmmoChanged;

	float GetDamage()const { return Damage; }
	void SetDamage(float SetDamage) { Damage = SetDamage; }
	int32 GetMagazineSize()const { return MagazineSize; }
	void SetMagazineSize(int32 SetMagazineSize) { MagazineSize = SetMagazineSize; }


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="===Weapon===|Properties")
    FText WeaponName = NSLOCTEXT("PriestHUD", "DefaultWeapon", "Weapon");

	UPROPERTY()
	TObjectPtr<UPartManager> PartManager;

	// Properties
	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	float Damage = 20.0f;

	// 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	float Range = 10000.0f;

	// 탄창에 들어가는 총알 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	int32 MagazineSize = 12;

	// 현재 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	int32 CurrentAmmo = 12;

	// 예비 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	int32 ReserveAmmo = 60;

	// 연사 속도 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	float FireInterval = 0.2f;

	// 재장전 소모 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Weapon===|Properties")
	float ReloadTime = 1.5f;

	// States
	// 재장전 상태
	bool bIsReloading = false;

	// 발사 가능 상태
	bool bCanFire = true;	

	// 재장전 타이머
	FTimerHandle ReloadTimerHandle;

	// 발사 쿨타임 타이머
	FTimerHandle FireTimerHandle;

protected:
	void CompleteReload();

	void ResetFire();

	// FPS
	void PerformTraceFPS(AIngamePlayerController* PlayerController, AActor* OwnerActor);

	// TPS
	void PerformTraceTPS(AIngamePlayerController* PlayerController, AActor* OwnerActor);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../UI/PriestHUDData.h"
#include "IngamePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UPriestHUDWidget;
class APlayerCharacter;
class AWeaponItem;

UCLASS()
class PROJECTPRIEST_API AIngamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AIngamePlayerController();
    UFUNCTION(Client, Unreliable)
    void ClientNotifyPlayerDamaged(APawn* DamagedPawn);

    // 데미지를 처리한 쪽에서 공격자의 컨트롤러로 전달하는 일회성 피드백.
    UFUNCTION(Client, Unreliable)
    void ClientNotifyHitConfirmed(float AppliedDamage, FVector DamageLocation);

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    TObjectPtr<UInputAction> GetMoveAction();
    TObjectPtr<UInputAction> GetLookAction();
    TObjectPtr<UInputAction> GetFireAction();
    TObjectPtr<UInputAction> GetThrowAction();
    TObjectPtr<UInputAction> GetSprintAction();
    TObjectPtr<UInputAction> GetJumpAction();
private:
    UFUNCTION()
    void HandleCombatPawnChanged(APawn* PreviousPawn, APawn* NewPawn);
    void RefreshCombatHUD();
    void UnbindCombatHUD();
    TWeakObjectPtr<APlayerCharacter> HUDPlayer;
    TWeakObjectPtr<AWeaponItem> HUDWeapon;
    FDelegateHandle HealthChangedHandle;
    FDelegateHandle AmmoChangedHandle;

protected:
    // BP_IngamePlayerController의 Class Defaults에서 WBP_PriestHUD를 지정한다.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Priest|UI")
    TSubclassOf<UPriestHUDWidget> HUDWidgetClass;

    // 실제 게임 데이터 연결 전의 초기 표시 값이다.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Priest|UI")
    FPriestHUDData InitialHUDData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> ThrowAction;
};

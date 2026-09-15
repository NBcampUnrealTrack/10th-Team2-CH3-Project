#pragma once
#include "CoreMinimal.h"
#include "MvcModel.h"
#include "../UI/PriestHUDData.h"
#include "PriestCombatModel.generated.h"
class APawn;
class APlayerCharacter;
class AWeaponItem;
UCLASS()
class PROJECTPRIEST_API UPriestCombatModel : public UObject, public IMvcModel
{
    GENERATED_BODY()
public:
    void SetPawn(APawn* Pawn);
    bool IsPlayerDead() const;
    void Disconnect();
    FPriestHUDData GetCombatData() const;
    virtual void BeginDestroy() override;
    virtual FDelegateHandle AddListener(UMvcControl* Control) override;
    virtual void RemoveListener(FDelegateHandle Handle) override;
    virtual void InvokePropertyChanged(uint8 PropertyName) override;
private:
    void Refresh();
    TWeakObjectPtr<APlayerCharacter> Player;
    TWeakObjectPtr<AWeaponItem> Weapon;
    FDelegateHandle HealthHandle;
    FDelegateHandle AmmoHandle;
    FModelChangedDelegate Changed;
};

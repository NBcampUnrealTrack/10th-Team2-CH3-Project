#include "PriestCombatModel.h"
#include "PlayerCharacter.h"
#include "WeaponItem.h"
#include "MvcControl.h"
void UPriestCombatModel::Disconnect()
{
    if (Player.IsValid())
    {
        Player->OnCombatChanged.Remove(HealthHandle);
    }
    if (Weapon.IsValid())
    {
        Weapon->OnAmmoChanged.Remove(AmmoHandle);
    }
    Player.Reset();
    Weapon.Reset();
    HealthHandle.Reset();
    AmmoHandle.Reset();
}
void UPriestCombatModel::BeginDestroy()
{
    Disconnect();
    Super::BeginDestroy();
}
void UPriestCombatModel::SetPawn(APawn* Pawn)
{
    Disconnect();
    Player = Cast<APlayerCharacter>(Pawn);
    if (Player.IsValid())
    {
        HealthHandle = Player->OnCombatChanged.AddUObject(this, &UPriestCombatModel::Refresh);
    }
    Refresh();
}
void UPriestCombatModel::Refresh()
{
    AWeaponItem* Current = Player.IsValid() ? Player->GetEquippedWeapon() : nullptr;
    if (!IsValid(Current))
    {
        Current = nullptr;
    }
    if (Weapon.Get() != Current)
    {
        if (Weapon.IsValid())
        {
            Weapon->OnAmmoChanged.Remove(AmmoHandle);
        }
        AmmoHandle.Reset();
        Weapon = Current;
        if (Current)
        {
            AmmoHandle = Current->OnAmmoChanged.AddUObject(this, &UPriestCombatModel::Refresh);
        }
    }
    InvokePropertyChanged(0); // Full combat snapshot.
}
FPriestHUDData UPriestCombatModel::GetCombatData() const
{
    FPriestHUDData Data;
    Data.Health = Player.IsValid() ? Player->GetCurrentHealth() : 0.0f;
    Data.MaxHealth = Player.IsValid() ? Player->GetMaxHealth() : 0.0f;
    Data.WeaponName = Weapon.IsValid() ? Weapon->GetWeaponName() : NSLOCTEXT("PriestHUD", "NoWeapon", "Unarmed");
    Data.MagazineAmmo = Weapon.IsValid() ? Weapon->GetCurrentAmmo() : 0;
    Data.ReserveAmmo = Weapon.IsValid() ? Weapon->GetReserveAmmo() : 0;
    return Data;
}
FDelegateHandle UPriestCombatModel::AddListener(UMvcControl* Control) { return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged); }
void UPriestCombatModel::RemoveListener(FDelegateHandle Handle) { Changed.Remove(Handle); }
void UPriestCombatModel::InvokePropertyChanged(uint8 PropertyName) { Changed.Broadcast(this, PropertyName); }

bool UPriestCombatModel::IsPlayerDead() const
{
    // A missing pawn is an initialization/unpossess state, not a death.
    return Player.IsValid() && FMath::IsFinite(Player->GetCurrentHealth()) && Player->GetCurrentHealth() <= 0.0f;
}

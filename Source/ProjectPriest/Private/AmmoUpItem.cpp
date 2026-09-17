#include "AmmoUpItem.h"
#include "PlayerCharacter.h"
#include "WeaponItem.h"
#include "JUtility.h"

AAmmoUpItem::AAmmoUpItem()
{
	ItemType = TEXT("AmmoUp");
}

void AAmmoUpItem::ActivateItem(AActor* Activator)
{
	JASSERT(Activator, "Actiavtor가 없습니다.");

	APlayerCharacter* Player = Cast<APlayerCharacter>(Activator);

	JASSERT(Player, "Player가 없습니다.");

	AWeaponItem* Weapon = Player->GetEquippedWeapon();

	JASSERT(Weapon, "Weapon이 없습니다.");

	Weapon->AddReserveAmmo(AmmoAmount);

	Super::ActivateItem(Activator);
}
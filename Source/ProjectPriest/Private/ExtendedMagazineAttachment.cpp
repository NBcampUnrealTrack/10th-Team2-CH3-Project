// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedMagazineAttachment.h"
#include "WeaponItem.h"


UExtendedMagazineAttachment::UExtendedMagazineAttachment()
{

}

void UExtendedMagazineAttachment::Equip(AWeaponItem* Weapon)
{
	if (!BaseMagazineSize)
	{
		BaseMagazineSize = Weapon->GetMagazineSize();
		
	}
	if (!bEquip)
	{
		Weapon->SetMagazineSize(BaseMagazineSize * 2);
		bEquip = true;
	}
}

void UExtendedMagazineAttachment::Unequip(AWeaponItem* Weapon)
{
	if (bEquip)
	{
		Weapon->SetMagazineSize(BaseMagazineSize);
		bEquip = false;
	}
}
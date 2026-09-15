// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelAttachment.h"
#include "WeaponItem.h"

UBarrelAttachment::UBarrelAttachment()
{

}

void UBarrelAttachment::Equip(AWeaponItem* Weapon)
{
	if (!BaseDamage)
	{
		BaseDamage = Weapon->GetDamage();
	}
	if(!bEquip)
	{
		Weapon->SetDamage(BaseDamage * 2.0f);
		bEquip = true;
	}
}

void UBarrelAttachment::Unequip(AWeaponItem* Weapon)
{
	if (bEquip)
	{
		Weapon->SetDamage(BaseDamage);
		bEquip = false;
	}
}

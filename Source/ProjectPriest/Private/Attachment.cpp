// Fill out your copyright notice in the Description page of Project Settings.

#include "Attachment.h"
#include "WeaponItem.h"
#include "Math/UnrealMathUtility.h"

UAttachment::UAttachment()
{
	DamageDelta = 0.0f;
	MagazineDelta = 0;
	//배율로 적용하니 - 값은 나올일이 없음
	StoredBaseDamage = -1.0f;
	StoredBaseMagazine = -1;
}

void UAttachment::ApplyToWeapon(AWeaponItem* Weapon)
{
	if (!Weapon || bEquip)
	{
		return;
	}
	Equip(Weapon);
}

void UAttachment::RemoveFromWeapon(AWeaponItem* Weapon)
{
	if (!Weapon || !bEquip)
	{
		return;
	}
	Unequip(Weapon);
}

void UAttachment::Equip(AWeaponItem* Weapon)
{
	if (!Weapon || bEquip)
	{
		return;
	}

	if (!FMath::IsNearlyZero(DamageDelta))
	{
		if (StoredBaseDamage < 0.0f)
		{
			StoredBaseDamage = Weapon->GetDamage();
		}
		Weapon->SetDamage(StoredBaseDamage * DamageDelta);
	}

	if (MagazineDelta != 0)
	{
		if (StoredBaseMagazine < 0)
		{
			StoredBaseMagazine = Weapon->GetMagazineSize();
		}
		Weapon->SetMagazineSize(StoredBaseMagazine * MagazineDelta);
	}
	bEquip = true;
}

void UAttachment::Unequip(AWeaponItem* Weapon)
{
	if (!Weapon || !bEquip)
	{
		return;
	}

	if (StoredBaseDamage >= 0.0f)
	{
		Weapon->SetDamage(StoredBaseDamage);
		StoredBaseDamage = -1.0f;
	}

	if (StoredBaseMagazine >= 0)
	{
		Weapon->SetMagazineSize(StoredBaseMagazine);
		StoredBaseMagazine = -1;
	}

	bEquip = false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment/PartManager.h"
#include "WeaponItem.h"
#include "JUtility.h"

//JLog("Invalid input, UPartManager::SetPart");
UPartManager::UPartManager()
{
}

void UPartManager::SetPart(const FPartData& Part, FPartData& ReplacedPart)
{
	FPartData* ExistingPart = PartInstance->PartSlots.Find(Part.SlotType);
	if (ExistingPart)
	{
		ReplacedPart = *ExistingPart;
	}
	PartInstance->PartSlots.Add(Part.SlotType, Part);
	return;
}

bool UPartManager::RemovePart(const FPartData& ReplacedPart)
{
	if (PartInstance->PartSlots.Remove(ReplacedPart.SlotType) > 0)
	{
		return true;
	}
	JLog("Invalid input, UPartManager::RemovePart");
	return false;
}

void UPartManager::SetPartInstance(UPartInstance* NewPartInstance)
{
	PartInstance = NewPartInstance;
}

void UPartManager::ApplyPartsToWeapon(AWeaponItem* Weapon)
{
	if (BaseDamage < 0)
	{
		BaseDamage = Weapon->GetDamage();
	}
	if (BaseMagazineSize < 0)
	{
		BaseMagazineSize = Weapon->GetMagazineSize();
	}

	float ValuDamage = 1.0f;
	float ValueMagazineSize = 1.0f;

	for (const auto& Part : PartInstance->PartSlots)
	{
		if (Part.Value.Damage > 0.0f)
		{
			ValuDamage *= Part.Value.Damage;
		}
		if (Part.Value.MagazineSize > 0.0f)
		{
			ValueMagazineSize *= Part.Value.MagazineSize;
		}
	}
	Weapon->SetDamage(BaseDamage * ValuDamage);
	Weapon->SetMagazineSize(BaseMagazineSize * ValueMagazineSize);
}

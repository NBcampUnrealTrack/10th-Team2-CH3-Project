// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachment.h"
#include "BarrelAttachment.generated.h"


class AWeaponItem;

UCLASS()
class PROJECTPRIEST_API UBarrelAttachment : public UAttachment
{
	GENERATED_BODY()
	
public:
	UBarrelAttachment();

protected:
	float BaseDamage = 0;

	virtual void Equip(AWeaponItem* Weapon);
	virtual void Unequip(AWeaponItem* Weapon);
};

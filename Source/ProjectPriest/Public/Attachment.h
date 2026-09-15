// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attachment.generated.h"

class AWeaponItem;

UCLASS()
class PROJECTPRIEST_API UAttachment : public UObject
{
	GENERATED_BODY()
	
public:
	UAttachment();

protected:
	bool bEquip = false;

	virtual void Equip(AWeaponItem* Weapon);
	virtual void Unequip(AWeaponItem* Weapon);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachment.h"
#include "ExtendedMagazineAttachment.generated.h"


class AWeaponItem;

UCLASS()
class PROJECTPRIEST_API UExtendedMagazineAttachment : public UAttachment
{
	GENERATED_BODY()
	
public:
	UExtendedMagazineAttachment();

protected:
	int32 BaseMagazineSize = 0;

	virtual void Equip(AWeaponItem* Weapon);
	virtual void Unequip(AWeaponItem* Weapon);
};

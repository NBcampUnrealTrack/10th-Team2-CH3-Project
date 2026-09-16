// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attachment.generated.h"

class AWeaponItem;

UCLASS(Blueprintable)
class PROJECTPRIEST_API UAttachment : public UObject
{
	GENERATED_BODY()
	
public:
	UAttachment();

	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void ApplyToWeapon(AWeaponItem* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void RemoveFromWeapon(AWeaponItem* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Attachment")
	bool IsEquipped() const { return bEquip; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment")
	bool bEquip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment|Properties")
	float DamageDelta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment|Properties")
	float MagazineDelta = 0;

	UPROPERTY(Transient)
	float StoredBaseDamage = -1.0f;

	UPROPERTY(Transient)
	int32 StoredBaseMagazine = -1;

	virtual void Equip(AWeaponItem* Weapon);
	virtual void Unequip(AWeaponItem* Weapon);
};
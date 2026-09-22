// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "CrossBow.generated.h"

UCLASS()
class PROJECTPRIEST_API ACrossBow : public AWeaponItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACrossBow();

protected:
	virtual void ActivateItem(AActor* Activator) override;
};

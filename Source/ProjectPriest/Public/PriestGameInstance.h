// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PriestGameInstance.generated.h"

class UPartInstance;
class UPartManager;

UCLASS()
class PROJECTPRIEST_API UPriestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPriestGameInstance();

	UPartInstance* GetOrCreatePartInstance(FName ItemType);


protected:
	virtual void Init() override;

	UPROPERTY()
	TMap<FName, TObjectPtr<UPartInstance>> PartInstances;
};
// Fill out your copyright notice in the Description page of Project Settings.


#include "PriestGameInstance.h"
#include "Attachment/PartInstance.h"

UPriestGameInstance::UPriestGameInstance()
{
}

UPartInstance* UPriestGameInstance::GetOrCreatePartInstance(FName ItemType)
{
    if (TObjectPtr<UPartInstance>* FoundInstance = PartInstances.Find(ItemType))
    {
        return FoundInstance->Get();
    }

    UPartInstance* NewInstance = NewObject<UPartInstance>(this);
    NewInstance->Initialize();

    PartInstances.Add(ItemType, NewInstance);

    return NewInstance;
}
void UPriestGameInstance::Init()
{
	Super::Init();
}
	
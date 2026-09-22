// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment/PartSubsystem.h"
#include "JUtility.h"

FWeaponPartSlots* UPartSubsystem::GetOrCreatePartInstance(FName WeaponName)
{
	if (FWeaponPartSlots* FoundPartSlots = WeaponPartSlots.Find(WeaponName))
    {
        return FoundPartSlots;
    }

    FWeaponPartSlots& NewPartSlots = WeaponPartSlots.Add(WeaponName);

    if (&NewPartSlots == nullptr)
    {
        JLog("nullptr NewPartSlots");
    }

    return &NewPartSlots;
}

FPartData UPartSubsystem::SetPart(FName WeaponName, FPartData& Part)
{
    FWeaponPartSlots* WeaponSlots = GetOrCreatePartInstance(WeaponName);
    return SetPart(WeaponSlots, Part);
}

FPartData UPartSubsystem::SetPart(FWeaponPartSlots* WeaponSlots, FPartData& Part)
{
    FPartData ReplacedPart;


    FPartData* ExPart = WeaponSlots->PartSlots.Find(Part.SlotType);

    if (ExPart)
    {
        ReplacedPart = *ExPart;
    }

    WeaponSlots->PartSlots.Add(Part.SlotType, Part);

    return ReplacedPart;
}

FPartData UPartSubsystem::RemovePart(FName WeaponName, EPartSlot SlotType)
{
    FWeaponPartSlots* WeaponSlots = GetOrCreatePartInstance(WeaponName);
    return RemovePart(WeaponSlots, SlotType);
}

FPartData UPartSubsystem::RemovePart(FWeaponPartSlots* WeaponSlots, EPartSlot SlotType)
{
    if (WeaponSlots == nullptr)
    {
        JLog("nullptr WeaponSlots");
        return FPartData();
    }

    FPartData* ExPart = WeaponSlots->PartSlots.Find(SlotType);

    if (ExPart)
    {
        FPartData RemovedPart = *ExPart;

        WeaponSlots->PartSlots.Remove(SlotType);

        return RemovedPart;
    }

    JLog("nullptr ExPart");
    return FPartData();
}

FPartData UPartSubsystem::GetEquippedPart(FName WeaponName, EPartSlot SlotType)
{
    FWeaponPartSlots* WeaponSlots = GetOrCreatePartInstance(WeaponName);

    return GetEquippedPart(WeaponSlots, SlotType);
}

FPartData UPartSubsystem::GetEquippedPart(FWeaponPartSlots* WeaponSlots, EPartSlot SlotType)
{
    if (WeaponSlots == nullptr)
    {
        JLog("nullptr WeaponSlots");
        return FPartData();
    }

    FPartData* ExPart = WeaponSlots->PartSlots.Find(SlotType);

    if (ExPart)
    {
        return *ExPart;
    }

    JLog("nullptr ExPart");
    return FPartData();
}

FWeaponPartSlots UPartSubsystem::GetEquippedParts(FName WeaponName)
{
    return WeaponPartSlots[WeaponName];
};

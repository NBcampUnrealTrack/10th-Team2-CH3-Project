// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment/PartModel.h"
#include "Attachment/PartSubsystem.h"
#include "MvcControl.h"
#include "JUtility.h"
#include "Engine/Engine.h"

void UPartModel::Initialize(UPartSubsystem* PartSubsy)
{
    Disconnect();

    JASSERT(IsValid(PartSubsy), "%hs: PartSubsystem이 유효하지 않습니다.", __FUNCTION__);

    PartSubsystem = PartSubsy;

    PartSubsystem->OnPartsChanged.AddUniqueDynamic(this, &UPartModel::Refresh);

    Refresh();
}

void UPartModel::Disconnect()
{
    if (IsValid(PartSubsystem))
    {
        PartSubsystem->OnPartsChanged.RemoveDynamic(this, &UPartModel::Refresh);
    }

    PartSubsystem = nullptr;
}

//bool UPriestInventoryModel::SetCategory(EItemCategory InCategory)
//{
//    if (!IsValid(Inventory))
//    {
//        return false;
//    }
//
//    switch (InCategory)
//    {
//    case EItemCategory::Weapon:
//    case EItemCategory::Ammo:
//    case EItemCategory::Consumable:
//    case EItemCategory::Material:
//        break;
//    default:
//        return false;
//    }
//
//    if (Data.SelectedCategory == InCategory)
//    {
//        return true;
//    }
//
//    Data.SelectedCategory = InCategory;
//
//    Refresh();
//    return true;
//}

void UPartModel::BeginDestroy()
{
    Disconnect();
    Super::BeginDestroy();
}


void UPartModel::Refresh()
{
    JASSERT(IsValid(PartSubsystem), "%hs: PartSubsystem이 유효하지 않습니다.", __FUNCTION__);

    Data.Parts.Reset();

    FName Temp = "Weapon";//무기 추가시 변경

    FPartData BarrelPart = PartSubsystem->GetEquippedPart(Temp, EPartSlot::Barrel);


    FPartSlotViewData BarrelViewData;

    BarrelViewData.StatName = FText::FromString( TEXT("데미지"));

    BarrelViewData.SlotType = EPartSlot::Barrel;
    BarrelViewData.Name = BarrelPart.Name;

    float DamagePercent = 0.0f;
    if (BarrelPart.Damage != -1.0f)
    {
        DamagePercent = (BarrelPart.Damage - 1.0f) * 100.0f;
    }
    BarrelViewData.StatValue = FText::Format(
        FText::FromString(TEXT("{0}%")),
        FText::AsNumber(DamagePercent));

    FPartData MagazinePart = PartSubsystem->GetEquippedPart(Temp, EPartSlot::Magazine);

    FPartSlotViewData MagazineViewData;

    MagazineViewData.StatName = FText::FromString( TEXT("탄창 크기"));

    MagazineViewData.SlotType = EPartSlot::Magazine;
    MagazineViewData.Name = MagazinePart.Name;

    float MagazinePercent = 0.0f;

    if (MagazinePart.MagazineSize != -1.0f)
    {
        MagazinePercent = (MagazinePart.MagazineSize - 1.0f) * 100.0f;
    }

    MagazineViewData.StatValue = FText::Format(
        FText::FromString(TEXT("{0}%")),
        FText::AsNumber(MagazinePercent));

    Data.Parts.Add(BarrelViewData);
    Data.Parts.Add(MagazineViewData);

    InvokePropertyChanged(0);
}

FDelegateHandle UPartModel::AddListener(UMvcControl* Control)
{
    return Changed.AddUObject(Control, &UMvcControl::HandleModelChanged);
}

void UPartModel::RemoveListener(FDelegateHandle Handle)
{
    Changed.Remove(Handle);
}

void UPartModel::InvokePropertyChanged(uint8 PropertyName)
{
    Changed.Broadcast(this, PropertyName);
}

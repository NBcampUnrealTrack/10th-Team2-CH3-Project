// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment/PartItem.h"
#include "Attachment/PartData.h"

APartItem::APartItem()
{
}

void APartItem::BeginPlay() 
{
	const FPartData* Data = PartData.GetRow<FPartData>(TEXT("PartItem"));
	ItemType = Data->Name;
}
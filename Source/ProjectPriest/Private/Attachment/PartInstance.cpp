// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment/PartInstance.h"
#include "Attachment/PartManager.h"

UPartInstance::UPartInstance()
{

}

void UPartInstance::Initialize()
{
	PartManager = NewObject<UPartManager>(this);
	PartManager->SetPartInstance(this);
}
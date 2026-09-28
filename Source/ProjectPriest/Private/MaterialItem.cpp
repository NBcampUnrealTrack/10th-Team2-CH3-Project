#include "MaterialItem.h"
#include "MaterialTable.h"
#include "Engine/DataTable.h"
#include "PriestInventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "JUtility.h"

AMaterialItem::AMaterialItem()
{
	ItemType = TEXT("Material");
}

void AMaterialItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	JASSERT(Activator, "Activator가 없습니다.");

	UGameInstance* Instance = Activator->GetGameInstance();

	JASSERT(IsValid(Instance), "GameInstance가 없습니다.");

    UPriestInventorySubsystem* Inventory =
        Instance->GetSubsystem<UPriestInventorySubsystem>();

    JASSERT(IsValid(Inventory), "InventorySubsystem이 없습니다.");

    FPriestItemData Data;

    JASSERT(Inventory->TryGetItemData(MaterialID, Data), "아이템 정의가 없습니다.");

    if (Inventory->AddItem(MaterialID, Quantity))
    {
        DestroyItem();
    }
}

const FMaterialTable* AMaterialItem::GetMaterialData() const
{
	if (!MaterialDataTable || MaterialID.IsNone())
	{
		return nullptr;
	}

	return MaterialDataTable->FindRow<FMaterialTable>(MaterialID, TEXT("MaterialItem"));
}
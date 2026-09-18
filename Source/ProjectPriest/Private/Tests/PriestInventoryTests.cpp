#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Engine/GameInstance.h"
#include "PriestInventorySubsystem.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/UnrealType.h"
#include "../../UI/PriestInventoryModel.h"
#include "../../UI/PriestQuickSlotModel.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPriestInventoryStackTest, "Priest.Inventory.StacksAndPreview",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPriestInventoryStackTest::RunTest(const FString& Parameters)
{
    TStrongObjectPtr<UGameInstance> Instance(NewObject<UGameInstance>());
    TStrongObjectPtr<UPriestInventorySubsystem> Inventory(NewObject<UPriestInventorySubsystem>(Instance.Get()));
    const FName Potion(TEXT("Potion.Health"));
    Inventory->GrantPreviewItemsOnce();
    TestEqual(TEXT("Preview has four kinds"), Inventory->GetOwnedItems().Num(), 4);
    TestTrue(TEXT("Same kind adds to existing stack"), Inventory->AddItem(Potion, FText::FromString(TEXT("Potion")), 3));
    TestEqual(TEXT("Stack quantity is combined"), Inventory->GetQuantity(Potion), 8);
    TestEqual(TEXT("Stack count unchanged"), Inventory->GetOwnedItems().Num(), 4);
    TestFalse(TEXT("Insufficient removal rejected"), Inventory->RemoveItem(Potion, 9));
    TestFalse(TEXT("Negative addition rejected"), Inventory->AddItem(Potion, FText::FromString(TEXT("Potion")), -1));
    TestFalse(TEXT("Overflow rejected"), Inventory->AddItem(Potion, FText::FromString(TEXT("Potion")), MAX_int32));
    TestEqual(TEXT("Rejected operations preserve quantity"), Inventory->GetQuantity(Potion), 8);
    TestTrue(TEXT("Consume one"), Inventory->RemoveItem(Potion, 1));
    Inventory->GrantPreviewItemsOnce();
    TestEqual(TEXT("Menu re-entry does not refill"), Inventory->GetQuantity(Potion), 7);
    TestTrue(TEXT("Remove final items"), Inventory->RemoveItem(Potion, 7));
    TestEqual(TEXT("Empty stack removed"), Inventory->GetOwnedItems().Num(), 3);
    TestEqual(TEXT("Missing item quantity is zero"), Inventory->GetQuantity(Potion), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPriestInventoryMvcTest, "Priest.Inventory.MvcLifecycle",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPriestInventoryMvcTest::RunTest(const FString& Parameters)
{
    TStrongObjectPtr<UGameInstance> Instance(NewObject<UGameInstance>());
    TStrongObjectPtr<UPriestInventorySubsystem> Inventory(NewObject<UPriestInventorySubsystem>(Instance.Get()));
    TStrongObjectPtr<UDataTable> Items(NewObject<UDataTable>());
    TStrongObjectPtr<UDataTable> Potions(NewObject<UDataTable>());
    Items->RowStruct = FPriestItemData::StaticStruct();
    Potions->RowStruct = FPotionData::StaticStruct();
    const FName Potion(TEXT("Test.Potion"));
    const FName Herb(TEXT("Test.Herb"));
    FPriestItemData ItemRow;
    ItemRow.Category = EItemCategory::Consumable;
    Items->AddRow(Potion, ItemRow);
    ItemRow.Category = EItemCategory::Material;
    Items->AddRow(Herb, ItemRow);
    Potions->AddRow(Potion, FPotionData{});

    // Supply isolated tables without changing production initialization or project assets.
    FObjectPropertyBase* ItemProperty = FindFProperty<FObjectPropertyBase>(Inventory->GetClass(), TEXT("ItemDataTable"));
    FObjectPropertyBase* PotionProperty = FindFProperty<FObjectPropertyBase>(Inventory->GetClass(), TEXT("PotionDataTable"));
    if (!TestNotNull(TEXT("Item table property"), ItemProperty)
        || !TestNotNull(TEXT("Potion table property"), PotionProperty))
    {
        return false;
    }
    ItemProperty->SetObjectPropertyValue_InContainer(Inventory.Get(), Items.Get());
    PotionProperty->SetObjectPropertyValue_InContainer(Inventory.Get(), Potions.Get());
    Inventory->AddItem(Potion, FText::FromString(TEXT("Potion")), 3);
    Inventory->AddItem(Herb, FText::FromString(TEXT("Herb")), 2);

    TStrongObjectPtr<UPriestInventoryModel> Model(NewObject<UPriestInventoryModel>());
    Model->Initialize(Inventory.Get());
    TestEqual(TEXT("Default category contains only potion"), Model->GetData().Items.Num(), 1);
    if (!Model->GetData().Items.IsEmpty())
    {
        TestTrue(TEXT("Potion supports quick slots"), Model->GetData().Items[0].bQuickSlotCompatible);
    }
    TestTrue(TEXT("Select material"), Model->SetCategory(EItemCategory::Material));
    TestTrue(TEXT("Repeated selection succeeds"), Model->SetCategory(EItemCategory::Material));
    TestFalse(TEXT("None rejected"), Model->SetCategory(EItemCategory::None));
    TestFalse(TEXT("Unknown category rejected"), Model->SetCategory(static_cast<EItemCategory>(255)));
    TestEqual(TEXT("Rejected selection preserves category"), Model->GetData().SelectedCategory, EItemCategory::Material);
    Inventory->AddItem(Herb, FText::FromString(TEXT("Herb")), 1);
    if (!TestEqual(TEXT("Material remains visible"), Model->GetData().Items.Num(), 1))
    {
        return false;
    }
    TestEqual(TEXT("Inventory notification refreshes quantity"), Model->GetData().Items[0].Quantity, 3);
    Model->Disconnect();
    Inventory->AddItem(Herb, FText::FromString(TEXT("Herb")), 1);
    TestEqual(TEXT("Disconnected model stops receiving changes"), Model->GetData().Items[0].Quantity, 3);
    TestFalse(TEXT("Disconnected model rejects requests"), Model->SetCategory(EItemCategory::Consumable));
    Model->Initialize(Inventory.Get());
    TestEqual(TEXT("Reconnect preserves selected category and refreshes"), Model->GetData().Items[0].Quantity, 4);

    TStrongObjectPtr<UPriestQuickSlotModel> Quick(NewObject<UPriestQuickSlotModel>());
    Quick->Initialize(Inventory.Get(), 0);
    TestFalse(TEXT("Material cannot be assigned"), Quick->AssignItem(Herb));
    TestTrue(TEXT("Potion can be assigned"), Quick->AssignItem(Potion));
    TestEqual(TEXT("Assigned quantity"), Quick->GetData().Quantity, 3);
    Inventory->RemoveItem(Potion, 3);
    TestEqual(TEXT("Consumption refreshes quantity to zero"), Quick->GetData().Quantity, 0);
    TestEqual(TEXT("Depleted potion keeps assignment"), Quick->GetData().ItemId, Potion);
    TestTrue(TEXT("Clear assignment"), Quick->ClearItem());
    TestTrue(TEXT("Cleared slot has no item"), Quick->GetData().ItemId.IsNone());
    Inventory->AddItem(Potion, FText::FromString(TEXT("Potion")), 2);
    Quick->AssignItem(Potion);
    Quick->Disconnect();
    Inventory->RemoveItem(Potion, 1);
    TestEqual(TEXT("Disconnected quick slot stops receiving changes"), Quick->GetData().Quantity, 2);
    TestFalse(TEXT("Disconnected quick slot rejects assignment"), Quick->AssignItem(Potion));
    Model->Disconnect();
    return true;
}

#endif

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Engine/GameInstance.h"
#include "PriestInventorySubsystem.h"
#include "UObject/StrongObjectPtr.h"

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

#endif

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MaterialItem.generated.h"

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	// 임의로 지은 이름, 차후 수정
	None,
	WhisperDropItem,
	GhostDropItem
};

UCLASS()
class PROJECTPRIEST_API AMaterialItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMaterialItem();

protected:
	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Material===|Properties")
	EMaterialType MaterialType = EMaterialType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Material===|Properties")
	int32 Quantity = 1;

protected:
	// Internal Functions
	virtual void ActivateItem(AActor* Activator) override;
};
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MvcEvents.generated.h"

class IMvcModel;
class IMvcView;

//////////////////////////////////////////////
/// EVENT FOR VIEW!
//////////////////////////////////////////////
UENUM(BlueprintType)
enum class EViewEventType : uint8
{
    None              UMETA(Hidden),
    ButtonClicked     UMETA(DisplayName = "ButtonClicked"),
    QuickSlotRequest  UMETA(DisplayName = "QuickSlotRequest"),
    InventoryRequest  UMETA(DisplayName = "InventoryRequest"),
    PartSlotRequest   UMETA(DisplayName = "PartSlotRequest")
};

UENUM(BlueprintType)
enum class EButtonName : uint8
{
    None              UMETA(Hidden),
};

UCLASS()
class PROJECTPRIEST_API UEventParameterBase : public UObject
{
    GENERATED_BODY()

public:
    EViewEventType GetEventType();

protected:
    EViewEventType EventType;
};

UCLASS()
class PROJECTPRIEST_API UButtonClickedEvent : public UEventParameterBase
{
    GENERATED_BODY()

public:
    static UButtonClickedEvent* Create(EViewEventType EventType, EButtonName ButtonName);

private:
    UButtonClickedEvent();

protected:
    EButtonName ButtonName;
};


//////////////////////////////////////////////
/// EVENT FOR MODEL!
//////////////////////////////////////////////

//PROPERTYIDE must be Enum

UENUM(BlueprintType)
enum class EModelName : uint8
{
    None        UMETA(Hidden)
};


DECLARE_MULTICAST_DELEGATE_TwoParams(FModelChangedDelegate, IMvcModel* Model, uint8 PropertyName);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FViewEventRaisedDelegate, IMvcView*, EViewEventType, UEventParameterBase*);
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"

UCLASS()
class PROJECTPRIEST_API UState : public UObject
{
    GENERATED_BODY()

public:
    virtual ~UState() = default;

    virtual void OnEnter();
    virtual void OnTick(float DeltaSeconds);
    virtual void OnExit();

    void SetDisplayName(const FString& DisplayName);
    const FString& GetDisplayName() const;

protected:
    FString DisplayName;
};
